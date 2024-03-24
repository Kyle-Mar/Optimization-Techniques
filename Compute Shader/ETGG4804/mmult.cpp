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

class Matrix{
  public:
    std::vector<float> M;
    int nr,nc;
    Matrix(int nr, int nc){
        M.resize(nr*nc);
        this->nr=nr;
        this->nc=nc;
    }
    float get(int r, int c) const {
        return M[r*this->nr + c];
    }
    void set(int r, int c, float f){
        M[r*this->nr + c] = f;
    }
};

inline
Matrix operator*(const Matrix& A, const Matrix& B)
{
    assert(A.nc == B.nr);
    Matrix C(A.nr,B.nc);
    for(int i=0;i<A.nr;++i){
        for(int j=0;j<B.nc;++j){
            float total = 0.0;
            for(int k=0;k<A.nc;++k){
                total += A.get(i,k) * B.get(k,j);
            }
            C.set(i,j,total);
        }
    }
    return C;
}

int main(int, char** )
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* win = SDL_CreateWindow("ETGG",
        10,10, 10,10, SDL_WINDOW_VULKAN | SDL_WINDOW_HIDDEN );



    //make two large matrices. This is just for testing;
    //we don't really care what values we get
    int nr=4000;
    int nc=nr;
    Matrix A(nr,nc);
    Matrix B(nr,nc);
    for(int i=0;i<nr;++i){
        for(int j=0;j<nc;++j){
            A.set(i,j,math2801::uniform());
            B.set(i,j,math2801::uniform());
        }
    }

    
    //StagingBuffer* MatrixB = new StagingBuffer(
    //    ctx,
    //    VK_BUFFER_USAGE_TRANSFER_SRC_BIT |
    //    VK_BUFFER_USAGE_TRANSFER_DST_BIT |
    //    VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
    //    B.M,        //initial 
    //    "staging2"       //for debugging
    //);
    
    Stopwatch s1;
    s1.start();
    Matrix C = A * B;
    s1.stop();
    std::cout << "CPU Time: " << s1.elapsed_ms() << " msec\n";


    //for debugging...
    if(0){
        std::cout << "A:\n";
        for(int i=0;i<nr;++i){
            for(int j=0;j<nc;++j){
                std::cout << A.get(i,j) << " ";
            }
            std::cout << "\n";
        }
        std::cout << "B:\n";
        for(int i=0;i<nr;++i){
            break;
            for(int j=0;j<nc;++j){
                std::cout << B.get(i,j) << " ";
            }
            std::cout << "\n";
        }
        std::cout << "C:\n";

        for(int i=0;i<nr;++i){
            for(int j=0;j<nc;++j){
                std::cout << C.get(i,j) << " ";
                break;
            }
            std::cout << "\n";
            break;
        }
    }

    Matrix gpuC(nr,nc);

    Stopwatch s2;
    s2.start();

    VkPhysicalDeviceFeatures features{};
    VulkanContext* ctx = new VulkanContext(win, features);
    auto [descriptorSet, pipeline] = utils::initializeComputePipeline(
        ctx,
        "test1.comp",//compute shader
        {
        DescriptorSetEntry{
            .type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
            .slot = 0
            }
        ,
        DescriptorSetEntry{
            .type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
            .slot = 1
            }
        ,
        DescriptorSetEntry{
            .type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
            .slot = 2
        },
        DescriptorSetEntry{
            .type=VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            .slot = 3
        },
        }
    );
    StagingBuffer* MatrixA = new StagingBuffer(
        ctx,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT |
        VK_BUFFER_USAGE_TRANSFER_DST_BIT |
        VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
        A.M,        //initial 
        "staging0"       //for debugging
    );

    StagingBuffer* MatrixB = new StagingBuffer(
        ctx,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT |
        VK_BUFFER_USAGE_TRANSFER_DST_BIT |
        VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
        B.M,        //initial 
        "staging1"       //for debugging
    );

    StagingBuffer* MatrixC = new StagingBuffer(
        ctx,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT |
        VK_BUFFER_USAGE_TRANSFER_DST_BIT |
        VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
        gpuC.M,        //initial 
        "staging2"       //for debugging
    );
    StagingBuffer* Size = new StagingBuffer(
        ctx,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT |
        VK_BUFFER_USAGE_TRANSFER_DST_BIT |
        VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
        &nc,
        sizeof(nc),
        "size_uniform"
    
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
    descriptorSet->setSlot(0, MatrixA->buffer);
    descriptorSet->setSlot(1, MatrixB->buffer);
    descriptorSet->setSlot(2, MatrixC->buffer);
    descriptorSet->setSlot(3, Size->buffer);
    descriptorSet->bind(cmd);
    vkCmdDispatch(cmd, (int)(nc/16)+1, (int)(nc/8)+1, 1);
    MatrixA->memoryBarrier(cmd);
    MatrixB->memoryBarrier(cmd);
    MatrixC->memoryBarrier(cmd);
    vkEndCommandBuffer(cmd);
    utils::endCompute(ctx, cmd);

    s2.stop();
    float* p = (float*)MatrixC->map();

    for (int i = 0; i < gpuC.M.size(); i++) {
        gpuC.M[i] = p[i];
    }


    //do GPU computation here...


    std::cout << "GPU Time: " << s2.elapsed_ms() << " msec\n";

    for(int i=0;i<nr;++i){
        for(int j=0;j<nc;++j){
            if( std::abs( C.get(i,j) - gpuC.get(i,j) ) >= 0.01 ){
                std::cout << "Mismatch at " << i << "," << j << ": " << C.get(i,j) << " " << gpuC.get(i,j) << "\n";
                return 1;
            }
        }
    }

    std::cout << "All OK\n";

    return 0;
}
