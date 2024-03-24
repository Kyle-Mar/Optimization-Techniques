#pragma once

#include <cstdint>
#include "vkhelpers.h"
#include "VertexManager.h"
#include "math2801.h"

//maximum number of characters on screen
#define MAX_CHARS 1000

class Framebuffer;
class Image;
class DeviceLocalBuffer;
class StagingBuffer;
class DescriptorSetLayout;
class DescriptorSetFactory;
class PushConstants;
class PipelineLayout;
class GraphicsPipeline;
class Sampler;
class DescriptorSet;

class Text{
  public:
    Text(VulkanContext* ctx, Framebuffer* framebuffer, std::string fontfile, std::string metricsfile);
    void appendCharacter(char ch, int x, int y);
    void cleanup();
    void clear();
    void print(int x, int y, int r, int g, int b, std::string s);
    void update(VkCommandBuffer cmd);
    void draw(VkCommandBuffer cmd);

  private:


    #pragma pack(push,1)
    struct CharData{
        std::uint32_t screenXY;
        std::uint32_t atlasXY;
        std::uint32_t ascentDescentWidthLeftBearing;
        std::uint32_t color;
    };
    #pragma pack(pop)

    struct Metrics{
        int ascent,descent,width,advance,x,y,leftbearing;
    };
    VulkanContext* ctx;
    std::map<char,Metrics> metrics;
    Image* textureAtlas;
    VertexManager* vertexManager;
    VertexManager::Info drawinfo;
    std::vector<CharData> textData;
    unsigned numChars;
    DeviceLocalBuffer* textBuffer;
    StagingBuffer* stagingBuffer;
    VkBufferView textView;
    DescriptorSetLayout* descriptorSetLayout;
    DescriptorSetFactory* descriptorSetFactory;
    DescriptorSet* descriptorSet;
    Sampler* sampler;
    PushConstants* pushConstants;
    math2801::vec2 screenSize;
    PipelineLayout* pipelineLayout;
    GraphicsPipeline* pipeline;
    bool dirty;
};
