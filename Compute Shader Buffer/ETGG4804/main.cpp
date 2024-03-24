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

void printHash(unsigned i, uint32_t h[8])
{
   printf("Chunk %d: %08x%08x%08x%08x%08x%08x%08x%08x\n",
       i,
       h[0],h[1],
       h[2],h[3],
       h[4],h[5],
       h[6],h[7]
       );
}


int main(int, char* argv[])
{
   SDL_Init(SDL_INIT_VIDEO);
   SDL_Window* win = SDL_CreateWindow("ETGG",
       10,10, 10,10, SDL_WINDOW_VULKAN | SDL_WINDOW_HIDDEN );

   char* filename = argv[1];
   uint32_t chunkSizeInBytes = atoi(argv[2]);

   std::ifstream file(filename);
   std::cout << std::filesystem::current_path() << std::endl;

   if (!file.good()) {
       throw std::runtime_error("Could not open file.");
   }
   else {
       std::cout << "opened file" << std::endl;
   }
   
   file.seekg(0, file.end);
   int size = file.tellg();
   file.seekg(0, file.beg);


   char* buffer = new char[size];
   file.read(buffer, size);

   //std::cout.write(buffer, size);

   uint32_t numChunks = size / chunkSizeInBytes;

   std::cout << "CHUNKSIZEINWORDS"<<chunkSizeInBytes / 4 << std::endl;

   std::vector<unsigned int> hashVec;
   hashVec.resize(numChunks * 8);

   Stopwatch swatch;
   swatch.start();
   VkPhysicalDeviceFeatures features{};
   VulkanContext* ctx = new VulkanContext(win, features);
   
   auto [descriptorSet, pipeline] = utils::initializeComputePipeline(
       ctx,
       "test2.comp",
       {
           DescriptorSetEntry{
           .type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
           .slot = 0,
           },
           DescriptorSetEntry{
           .type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
           .slot = 1,
           },
           DescriptorSetEntry{
           .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
           .slot = 3,
           },
       }
   );

   std::vector<char> input(buffer, buffer + size);
   DeviceLocalBuffer* inputData = new DeviceLocalBuffer(
       ctx,
       nullptr,
       input.size() * sizeof(char),
       VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
       "input"
   );

   DeviceLocalBuffer* hashData = new DeviceLocalBuffer(
       ctx,
       nullptr,
       hashVec.size() * sizeof(uint32_t),
       VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
       "hash"
   );
   inputData->copyFromCPUMemory(input.data(), 0, input.size() * sizeof(char));
   hashData->copyFromCPUMemory(hashVec.data(), 0, hashVec.size() * sizeof(uint32_t));


   int numWords = chunkSizeInBytes / 4;
   StagingBuffer* sizeBuffer = new StagingBuffer(
       ctx,
       VK_BUFFER_USAGE_TRANSFER_SRC_BIT |
       VK_BUFFER_USAGE_TRANSFER_DST_BIT |
       VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
       &numWords,
       sizeof(numWords),
       "size_uniform"
   );
   utils::beginCompute(ctx);
   std::cout << numChunks << std::endl;
   auto cmd = CommandBuffer::allocate();
   vkBeginCommandBuffer(
       cmd,
       VkCommandBufferBeginInfo{
           .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
           .pNext = nullptr,
           .flags = 0,
           .pInheritanceInfo = nullptr,
       }
   );
   pipeline->use(cmd);
   descriptorSet->setSlot(0, inputData->buffer);
   descriptorSet->setSlot(1, hashData->buffer);
   descriptorSet->setSlot(3, sizeBuffer->buffer);
   descriptorSet->bind(cmd);
   vkCmdDispatch(cmd, (numChunks / 64)+1, 1, 1);
   inputData->memoryBarrier(cmd);
   hashData->memoryBarrier(cmd);
   vkEndCommandBuffer(cmd);


   utils::endCompute(ctx, cmd);
   


   hashData->copyToCPUMemory(hashVec.data(), 0, hashVec.size() * sizeof(uint32_t));
   uint32_t* p = hashVec.data();
   uint32_t* h = p;
   swatch.stop();

   for(unsigned i=0;i<numChunks && i < 4;i++){
       h = p + i*8;
       printHash(i,h);
   }
   for(unsigned i=5;i<numChunks && i < 20;i+=5){
       h = p + i*8;
       printHash(i,h);
   }
   for(unsigned i=20;i<numChunks && i < 100;i+=20){
       h = p + i*8;
       printHash(i,h);
   }
   for(unsigned i=100;i<numChunks;i+=100){
       h = p + i*8;
       printHash(i,h);
   }
   std::cout << std::endl;
   std::cout << "DISPATCH COUNT: " << int(size / 64) + 1 << std::endl;
   std::cout << "WORDS PER CHUNK: " << chunkSizeInBytes / 4 << std::endl;
   std::cout << "NUM CHUNKS: " << numChunks << std::endl;
   std::cout << "ELAPSED: " << swatch.elapsed_ms() << "ms" << std::endl;
   std::cout << std::endl;

   inputData->cleanup();
   hashData->cleanup();
   sizeBuffer->cleanup();
   CleanupManager::cleanupEverything();
   ctx->cleanup();

   delete[] buffer;

   return 0;
}
