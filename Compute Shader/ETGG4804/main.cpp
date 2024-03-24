#include "vkhelpers.h"
#include "ComputePipeline.h"
#include "CommandBuffer.h"
#include "Buffers.h"
#include "Descriptors.h"
#include "CleanupManager.h"
#include "utils.h"
#include <iostream>
#include <SDL.h>
#include <utils.h>
#include <framework/utils.cpp>

int main(int , char** )
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* win = SDL_CreateWindow("ETGG",
        10,10, 10,10, SDL_WINDOW_VULKAN | SDL_WINDOW_HIDDEN );
    VkPhysicalDeviceFeatures features{};
    VulkanContext* ctx = new VulkanContext(win,features);

    auto [descriptorSet,pipeline] = utils::initializeComputePipeline(
        ctx,
        "test1.comp",       //compute shader
        {
            DescriptorSetEntry{
                .type=VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
                .slot=0
            }
        }
    );

    //~ ctx->printHeapSizes();

    StagingBuffer* slowBuffer = new StagingBuffer(
        ctx,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT |
        VK_BUFFER_USAGE_TRANSFER_DST_BIT |
        VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
        nullptr,        //initial data
        1024,           //size
        "staging"       //for debugging
    );

    utils::beginCompute(ctx);
    auto cmd = CommandBuffer::allocate();
    vkBeginCommandBuffer(
        cmd,
        VkCommandBufferBeginInfo{
            .sType=VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .pNext=nullptr,
            .flags=0,
            .pInheritanceInfo=nullptr
        }
    );
    pipeline->use(cmd);
    descriptorSet->setSlot( 0, slowBuffer->buffer );
    descriptorSet->bind(cmd);
    vkCmdDispatch(cmd,10,1,1);
    slowBuffer->memoryBarrier(cmd);
    vkEndCommandBuffer(cmd);
    utils::endCompute(ctx,cmd);

    std::int32_t* p = (std::int32_t*) slowBuffer->map();
    for(int i=0;i<10;++i){
        std::cout << p[i] << " ";
    }
    std::cout << "\n";
    slowBuffer->unmap();

    slowBuffer->cleanup();
    CleanupManager::cleanupEverything();
    ctx->cleanup();

    return 0;
}
