#include "Image.h"
#include <iostream>
#include <cmath>
#include <cassert>
#include "Stopwatch.h"

//for debugging...
#ifndef VERBOSE
    #define VERBOSE 0
#endif

inline                
bool chunksAreDifferent( std::uint8_t* A, std::uint8_t* B, int chunkThreshold, int w)
{            
    //sum of absolute differences for all 64 pixels in the chunk
    int totalDiff = 0;

    for(int y=0;y<8;++y){
        for(int x=0;x<8;++x){
            totalDiff += std::abs(A[x] - B[x]);
        }
        A += w;
        B += w;
    }
    
    #if VERBOSE
        std::cout << "Total difference: " << totalDiff << "\n";
    #endif
    return totalDiff > chunkThreshold;
}

/*int main(int argc, char* argv[])
{
    GreyscaleImage imgA(argv[1]);
    GreyscaleImage imgB(argv[2]);
    int chunkThreshold = atoi(argv[3]);     //if two chunks differ by this much: They are "not similar"
    
    //verify sizes match
    assert( imgA.width() == imgB.width() );
    assert( imgA.height() == imgB.height() );

    //shorten our code a bit
    int w = imgA.width();
    int h = imgA.height();
    
    //easier if we don't have to worry about leftovers
    assert( w % 16 == 0 );
    assert( h % 16 == 0 );
    
    Stopwatch sw;
    sw.start();
    
    //start of row in image A and image B
    std::uint8_t* rowA = imgA.pixels();
    std::uint8_t* rowB = imgB.pixels();

    //total number of chunks that don't match
    int numDissimilarChunks=0;
    
    //cycle over each chunk; each chunk is 8x8
    for(int y=0;y<h; y += 8){
        
        //pointer to upper left corner of the current 8x8 chunk
        auto upperLeftCornerA = rowA;
        auto upperLeftCornerB = rowB;
        
        for(int x=0;x<w; x+= 8 ){

            #if VERBOSE
                std::cout << "Considering chunk starting at " << x << "," << y << "...\n";
            #endif
                
            if( chunksAreDifferent( upperLeftCornerA, upperLeftCornerB, chunkThreshold, w ) )
                ++numDissimilarChunks;
            
            upperLeftCornerA += 8;
            upperLeftCornerB += 8;
        }
        
        rowA += w*8;
        rowB += w*8;
    }
    
    sw.stop();
    
    std::cout << "Num dissimilar chunks: " << numDissimilarChunks << "\n";
    std::cout << "Time: " << sw.elapsed_us() << " microsec\n";
    
    return 0;
}*/