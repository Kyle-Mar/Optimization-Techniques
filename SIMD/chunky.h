#pragma once

#include "ymm.h"
#include <iostream>

//~ static const ymmu8 shuffleOrder(
                          //~ 0,4,2,6, 1,5,3,7, 8,12,10,14, 9,13,11,15,
                          //~ 0,4,2,6, 1,5,3,7, 8,12,10,14, 9,13,11,15);
static const ymmu8 shuffleOrder(
    15,11,13,9,  14,10,12,8,  7,3,5,1,  6,2,4,0,
    15,11,13,9,  14,10,12,8,  7,3,5,1,  6,2,4,0
);

//static const ymmi32 permuteOrder( 0,4,1,5,2,6,3,7 );
static const ymmi32 permuteOrder(
    7,3,6,2,5,1,4,0
);

inline void chunky_rgba_to_planes(
    void* mem,
    ymmu8& RP, ymmu8& GP, ymmu8& BP, ymmu8& AP)
{
    std::uint8_t* p = (std::uint8_t*)mem;
    ymmu8 a(p);
    p+=32;
    ymmu8 b(p);
    p+=32;
    ymmu8 c(p);
    p+=32;
    ymmu8 d(p);
    p+=32;
    ymmi16 t1  = unpacklo( a,b );
    ymmi16 t2  = unpackhi( a,b );
    ymmi32 t3  = unpacklo( t1,t2 );
    ymmi32 t4  = unpackhi( t1,t2 );
    ymmi64 t5  = unpacklo( t3,t4 );
    ymmi64 t6  = unpackhi( t3,t4 );
    ymmi16 t7  = unpacklo(c,d);
    ymmi16 t8  = unpackhi(c,d);
    ymmi32 t9  = unpacklo( t7,t8 );
    ymmi32 t10 = unpackhi( t7,t8 );
    ymmi64 t11 = unpacklo( t9,t10 );
    ymmi64 t12 = unpackhi( t9,t10 );
    ymmi64 R = unpacklo( t5,t11 );
    ymmi64 G = unpackhi( t5,t11 );
    ymmi64 B = unpacklo( t6,t12 );
    ymmi64 A = unpackhi( t6,t12 );
    ymmi32 RS = R.shuffle( shuffleOrder );
    ymmi32 GS = G.shuffle( shuffleOrder );
    ymmi32 BS = B.shuffle( shuffleOrder );
    ymmi32 AS = A.shuffle( shuffleOrder );
    RP = RS.permute(permuteOrder).X;    //use .X to convert to xmmu8
    GP = GS.permute(permuteOrder).X;
    BP = BS.permute(permuteOrder).X;
    AP = AS.permute(permuteOrder).X;
}


inline void planes_to_chunky_rgba(
    const ymmu8& RP, const ymmu8& GP, const ymmu8& BP, const ymmu8& AP,
    void* mem)
{
    ymmi16 x1 = ymmi16(unpacklo( RP,GP ).X);
    ymmi16 x2 = ymmi16(unpacklo( BP,AP ).X);
    auto x3 = unpacklo( x1,x2 );
    auto x4 = unpackhi( x1,x2 );
    auto x5 = ymmi16(unpackhi( RP,GP ).X);
    auto x6 = ymmi16(unpackhi( BP,AP ).X);
    auto x7 = unpacklo( x5,x6 ).X;
    auto x8 = unpackhi( x5,x6 ).X;
    char* ptr = (char*) mem;
    _mm256_storeu2_m128i( (__m128i*)(ptr+64), (__m128i*)(ptr   ), x3 );
    _mm256_storeu2_m128i( (__m128i*)(ptr+80), (__m128i*)(ptr+16), x4 );
    _mm256_storeu2_m128i( (__m128i*)(ptr+96), (__m128i*)(ptr+32), x7 );
    _mm256_storeu2_m128i( (__m128i*)(ptr+112), (__m128i*)(ptr+48), x8 );
}
