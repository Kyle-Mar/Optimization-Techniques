#include <iostream>
#include "Stopwatch.h"
#include "vkhelpers.h"
#include "ComputePipeline.h"
#include "CommandBuffer.h"
#include "Buffers.h"
#include "Descriptors.h"
#include "CleanupManager.h"
#include "math2801.h"
#include <SDL.h>
#include <vector>
#include <cassert>
#include <utils.h>
#include <fstream>
#include <filesystem>
#include "Stopwatch.h"
#include "ReadScene.h"
#include "Uniforms.h"
#include <imageencode.h>

int main(int, char* argv[])
{
    using namespace math2801;


    GPUWorld world;
    readScene("scene.txt", &world);


    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* win = SDL_CreateWindow("ETGG",
        10,10, 10,10, SDL_WINDOW_VULKAN | SDL_WINDOW_HIDDEN );
    VkPhysicalDeviceFeatures features{};
    VulkanContext* ctx = new VulkanContext(win, features);
    

    // slot 0 : uniforms
    // slot 1 : input
    // slot 2 : output
    auto [descriptorSet, pipeline] = utils::initializeComputePipeline(
        ctx,
        "raytrace.comp",
        {

            DescriptorSetEntry{
            .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            .slot = 0,
            },
            DescriptorSetEntry{
            .type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
            .slot = 1,
            },
            DescriptorSetEntry{
            .type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
            .slot = 2,
            },
        }
    );

    const int width = 512;
    const int height = 512;

    float tan_theta = std::tan(35.0f / 180.0f * 3.14159265358979323f);
    Uniforms* uniforms = new Uniforms(ctx, 0, "uniforms.txt", "uniforms");
    uniforms->set("screenSize", ivec2(width, height));
    uniforms->set("eye", vec3(0, 0, 5));
    uniforms->set("up", vec3(0, 1, 0));
    uniforms->set("right", vec3(1, 0, 0));
    uniforms->set("look", vec3(0, 0, -1));
    uniforms->set("tan_theta", tan_theta);
    uniforms->set("lightPosition", vec3(5, 5, 5));
    uniforms->set("delta",
        vec2(
            2.0f * tan_theta / (width - 1.0f),
            2.0f * tan_theta / (height - 1.0f)
        )
    );

    DeviceLocalBuffer* triangleBuff = new DeviceLocalBuffer(ctx,
        world.triangles,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
        "triangles"
    );

    DeviceLocalBuffer* framebuffer = new DeviceLocalBuffer(ctx,
        nullptr,
        width * height * 4,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
        "framebuffer"
    );

    utils::beginCompute(ctx);
    auto cmd = CommandBuffer::allocate();
    vkBeginCommandBuffer(
        cmd,
        VkCommandBufferBeginInfo{
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .pNext = nullptr,
            .flags = 0,
            .pInheritanceInfo = nullptr
        }
    );
    pipeline->use(cmd);
    uniforms->bind(cmd, descriptorSet);
    descriptorSet->setSlot(1, triangleBuff->buffer);
    descriptorSet->setSlot(2, framebuffer->buffer);
    descriptorSet->bind(cmd);

    vkCmdDispatch(cmd, width / 64, height, 1);

    framebuffer->memoryBarrier(cmd);

    vkEndCommandBuffer(cmd);

    utils::endCompute(ctx, cmd);

    
    std::vector<char> pixels(width * height * 4);
    framebuffer->copyToCPUMemory(pixels.data(), 0, pixels.size());
    std::vector<char> png;
    png = imageencode::encodePNG(width, height, "RGBA8", pixels);

    std::ofstream out("out.png", std::ios::binary);
    assert(out.good());
    out.write(png.data(), png.size());
    out.close();

    framebuffer->cleanup();
    triangleBuff->cleanup();
    CleanupManager::cleanupEverything();
    ctx->cleanup();





    return 0;
}
