// //Ref:
// //http://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.180-4.pdf

// //input: "abc"
// //output: ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad

// #define _CRT_SECURE_NO_WARNINGS
// #include <stdio.h>
// #include <assert.h>
// #include <string.h>
// #include <stdint.h>
// #include <stdlib.h>
// #include <math.h>
// #include <stdint.h>
// #include <iostream>
// #include <ETGG4804/Stopwatch.h>
// //#include "Stopwatch.h"

// //a couple of typedefs for convenience
// typedef uint32_t uint;
// typedef uint64_t uint64;

// //SHA constants as specified by NIST. Computed via computeK.c
// //Fractional part of cube roots of first 64 primes
// uint K[64] = {
//     0x428a2f98 , 0x71374491 , 0xb5c0fbcf , 0xe9b5dba5 ,
//     0x3956c25b , 0x59f111f1 , 0x923f82a4 , 0xab1c5ed5 ,
//     0xd807aa98 , 0x12835b01 , 0x243185be , 0x550c7dc3 ,
//     0x72be5d74 , 0x80deb1fe , 0x9bdc06a7 , 0xc19bf174 ,
//     0xe49b69c1 , 0xefbe4786 , 0x0fc19dc6 , 0x240ca1cc ,
//     0x2de92c6f , 0x4a7484aa , 0x5cb0a9dc , 0x76f988da ,
//     0x983e5152 , 0xa831c66d , 0xb00327c8 , 0xbf597fc7 ,
//     0xc6e00bf3 , 0xd5a79147 , 0x06ca6351 , 0x14292967 ,
//     0x27b70a85 , 0x2e1b2138 , 0x4d2c6dfc , 0x53380d13 ,
//     0x650a7354 , 0x766a0abb , 0x81c2c92e , 0x92722c85 ,
//     0xa2bfe8a1 , 0xa81a664b , 0xc24b8b70 , 0xc76c51a3 ,
//     0xd192e819 , 0xd6990624 , 0xf40e3585 , 0x106aa070 ,
//     0x19a4c116 , 0x1e376c08 , 0x2748774c , 0x34b0bcb5 ,
//     0x391c0cb3 , 0x4ed8aa4a , 0x5b9cca4f , 0x682e6ff3 ,
//     0x748f82ee , 0x78a5636f , 0x84c87814 , 0x8cc70208 ,
//     0x90befffa , 0xa4506ceb , 0xbef9a3f7 , 0xc67178f2
// };

// //The SHA Ch (choose) function
// uint Ch(uint x, uint y, uint z){
//     return (x&y)^(~x&z);
// }

// //The SHA Maj (majority) function
// uint Maj(uint x, uint y, uint z){
//     return (x&y)^(x&z)^(y&z);
// }

// //Rotate right
// uint ROTR(uint n,uint x){
//     return (x>>n) | (x<<(32-n));
// }

// //Shift right
// #define SHR(n,x) ( (x) >> (n) )

// //The SHA Sigma-0 function
// uint Sigma0256(uint x){
//     return ROTR(2,x) ^ ROTR(13,x) ^ ROTR(22,x);
// }

// //The SHA Sigma-1 function
// uint Sigma1256(uint x){
//     return ROTR(6,x) ^ ROTR(11,x) ^ ROTR(25,x);
// }

// //The SHA sigma-0 function
// uint sigma0256(uint x){
//     return ROTR(7,x) ^ ROTR(18,x) ^ SHR(3,x);
// }

// //The SHA sigma-1 function
// uint sigma1256(uint x){
//     return ROTR(17,x) ^ ROTR(19,x) ^ SHR(10,x);
// }


// //convert v from little endian to big endian
// uint bigEndian(uint v)
// {
//     return (
//         ( v << 24                 ) |
//         ( ( v & 0x0000ff00 ) << 8 ) |
//         ( ( v & 0x00ff0000 ) >> 8 ) |
//         ( v >> 24                 )
//     );
// }

// //The meat of the SHA algorithm
// //This will process 64 bytes of input and update H
// //with the running total of the hash.
// void shaHelper(uint M[], uint Mindex, uint H[8])
// {
//     //M=Message (input)
//     //Mindex = index in M where we want to start processing
//     //H=output hash

//     uint W[64];
//     for(uint t=0;t<16;++t){
//         //need to treat M as sequence of big-endian ints
//         W[t] =  bigEndian(M[Mindex+t]);
//     }

//     for(uint t=16;t<64;++t){
//         W[t] = sigma1256(W[t-2]) +
//             W[t-7] +
//             sigma0256(W[t-15]) +
//             W[t-16];
//     }

//     uint a,b,c,d,e,f,g,h;
//     a=H[0];
//     b=H[1];
//     c=H[2];
//     d=H[3];
//     e=H[4];
//     f=H[5];
//     g=H[6];
//     h=H[7];
//     for(uint t=0;t<64;++t){
//         uint T1 = h + Sigma1256(e)+Ch(e,f,g)+K[t]+W[t];
//         uint T2 = Sigma0256(a) + Maj(a,b,c);
//         h=g;
//         g=f;
//         f=e;
//         e=d+T1;
//         d=c;
//         c=b;
//         b=a;
//         a=T1+T2;
//     }
//     H[0]+=a;
//     H[1]+=b;
//     H[2]+=c;
//     H[3]+=d;
//     H[4]+=e;
//     H[5]+=f;
//     H[6]+=g;
//     H[7]+=h;
// }


// //Compute SHA hash, including trailing padding
// void sha256(uint M[], uint numWords, uint H[8])
// {
//     //M=message (input data)
//     //numWords = number of words of data in M
//     //H=output hash

//     //initialize hash to predefined constants from NIST standard
//     H[0] = 0x6a09e667;
//     H[1] = 0xbb67ae85;
//     H[2] = 0x3c6ef372;
//     H[3] = 0xa54ff53a;
//     H[4] = 0x510e527f;
//     H[5] = 0x9b05688c;
//     H[6] = 0x1f83d9ab;
//     H[7] = 0x5be0cd19;

//     //hash the message
//     //We assume message is multiple of 64 bytes in size.
//     for(uint i=0;i<numWords;i+=16){
//         shaHelper(M,i,H);
//     }

//     //End of message is padded like so:
//     //byte 0x80, then 55 bytes with value 0x00, then
//     //8 byte big-endian integer specifying number of *bits*
//     //in the total message. This is a total of 64 bytes = 16 uints.
//     //This assumes the message size is a multiple of 64 bytes.
//     //If it isn't then the 55 should be different.
//     //For convenience, we assume the message is always a multiple of
//     //64 bytes.

//     uint padding[16];       //64 bytes
//     padding[0] = bigEndian(0x80000000);
//     for(int i=1;i<=13;++i){
//         padding[i]=0;
//     }

//     //Hint: In GLSL, see umulExtended()
//     uint64 sizeInBits = numWords * 4 * 8;

//     padding[14] = bigEndian(sizeInBits>>32);
//     padding[15] = bigEndian(sizeInBits&0xffffffff);

//     shaHelper(padding,0,H);

//     return;
// }


// void readFile(const char* filename, char** data, uint* size)
// {
//     FILE* fp = fopen(filename,"rb");
//     if(!fp){
//         printf("Cannot read file %s\n",filename);
//         exit(1);
//     }
//     fseek(fp,0,SEEK_END);
//     *size = (uint)ftell(fp);
//     fseek(fp,0,SEEK_SET);
//     *data = (char*) malloc(*size);
//     size_t nr = fread(*data,1,*size,fp);
//     if( nr != *size ){
//         printf("Read size mismatch:  %lu %d\n",nr,*size);
//         exit(1);
//     }
//     fclose(fp);
// }



// void printHash(unsigned i, uint h[8])
// {
//     printf("Chunk %d: %08x%08x%08x%08x%08x%08x%08x%08x\n",
//         i,
//         h[0],h[1],
//         h[2],h[3],
//         h[4],h[5],
//         h[6],h[7]
//         );
// }

// int main(int argc, char* argv[])
// {
//     char* data;
//     uint size;

//     const char* filename = argv[1];
//     uint chunkSizeInBytes = atoi(argv[2]);
//     readFile(filename, &data, &size);

//     if( chunkSizeInBytes % 64 ){
//         printf("Chunk size must be a multiple of 64\n");
//         return 1;
//     }

//     printf("Chunk size: %d bytes\n",chunkSizeInBytes);

//     unsigned numChunks = size / chunkSizeInBytes;
//     printf("%d chunks\n",numChunks);


//     if( size % chunkSizeInBytes != 0 ){
//         printf("Warning: Data length is not a multiple of %d; final %d bytes will be ignored\n",
//             chunkSizeInBytes, size - numChunks*chunkSizeInBytes );
//     }


//     Stopwatch swatch;
//     swatch.start();

//     uint* hashes = new uint[numChunks*8];
//     uint* p = (uint*) data;
//     uint* h = hashes;
//     uint chunkSizeInWords = chunkSizeInBytes/4;

//     std::cout << chunkSizeInWords << std::endl;
//     for(unsigned i=0;i<numChunks;++i){
//         sha256( p,chunkSizeInWords,h);
//         p += chunkSizeInWords;
//         for (int i = 0; i < 8; i++) {
//             //std::cout << "0x" << std::hex << *(h+i) << std::endl;
//         }
//         h += 8;
//     }
//     swatch.stop();



//     //only print a subset of hashes to avoid spamming the screen
//     for(unsigned i=0;i<numChunks && i < 4;i++){
//         h = hashes + i*8;
//         printHash(i,h);
//     }
//     for(unsigned i=5;i<numChunks && i < 20;i+=5){
//         h = hashes + i*8;
//         printHash(i,h);
//     }
//     for(unsigned i=20;i<numChunks && i < 100;i+=20){
//         h = hashes + i*8;
//         printHash(i,h);
//     }
//     for(unsigned i=100;i<numChunks;i+=100){
//         h = hashes + i*8;
//         printHash(i,h);
//     }

//     printf("\n\n%d msec\n\n", (int)swatch.elapsed_ms());
//     return 0;
// }
