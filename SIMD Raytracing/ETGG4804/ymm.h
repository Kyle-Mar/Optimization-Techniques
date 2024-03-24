#pragma once

#include <cstdint>
#include <ostream>
#include "xmm.h"

#ifdef _MSC_VER
    #include <intrin.h>
    #include <immintrin.h>
#else
    #include <mmintrin.h> //mmx
    #include <xmmintrin.h> //sse
    #include <emmintrin.h> //sse2
    #include <pmmintrin.h> //sse3
    #include <tmmintrin.h> //ssse3
    #include <smmintrin.h> //sse4.1
    #include <nmmintrin.h> //sse4.2
    #include <immintrin.h> //avx, avx2
#endif

class ymmu8;
class ymmi8;
class ymmi16;
class ymmi32;
class ymmi64; 


class ymm{
  public:
    __m256 X;
    ymm(){}
    ymm( const float* f ){
        this->X = _mm256_loadu_ps(f);
    }

    ymm( float f ){
        this->X = _mm256_set1_ps(f);
    }

    ymm(float a, float b, float c, float d,
        float e, float f, float g, float h ){
        this->X = _mm256_set_ps(h,g,f,e,d,c,b,a);
    }

    operator __m256() {
        return X;
    }

    operator ymmi32() const;

    ymm( __m256 m ){
        this->X=m;
    }

    void operator=(const ymm& maroscher) {
        this->X = maroscher.X;
    }

    static ymm zeros(){
        return ymm(_mm256_setzero_ps());
    }
    ymm blend(const ymm& b, const ymm& c) {
        return ymm(_mm256_blendv_ps(this->X, b.X, c.X));
    }
    ymm rsqrt() {
        return ymm(_mm256_rsqrt_ps(this->X));
    }

    void store(float* f){
        _mm256_storeu_ps(f, this->X);
    }

};

inline ymm min(const ymm& a, const ymm& b) {
    return ymm(_mm256_min_ps(a.X, b.X));
}

inline ymm max(const ymm& a, const ymm& b) {
    return ymm(_mm256_max_ps(a.X, b.X));
}

inline ymm operator+(const ymm& a, const ymm& b)
{
    return ymm( _mm256_add_ps( a.X, b.X ) );
}

inline ymm operator-(const ymm& a, const ymm& b)
{
    return ymm( _mm256_sub_ps( a.X, b.X ) );
}

inline ymm operator*(const ymm& a, const ymm& b)
{
    return ymm( _mm256_mul_ps( a.X, b.X ) );
}

inline ymm operator/(const ymm& a, const ymm& b)
{
    return ymm( _mm256_div_ps( a.X, b.X ));
}

inline ymm sqrt(const ymm& a, const ymm& b)
{
    return ymm( _mm256_sqrt_ps( a.X ));
}

inline ymm operator> (const ymm& a, const ymm& b) {
    return ymm(_mm256_cmp_ps(a.X, b.X, _CMP_GT_OQ));
}

inline ymm operator< (const ymm& a, const ymm& b) {
    return ymm(_mm256_cmp_ps(a.X, b.X, _CMP_LT_OQ));
}

inline ymm operator& (const ymm& a, const ymm& b) {
    return ymm(_mm256_and_ps(a.X, b.X));
}

inline ymm operator>= (const ymm& a, const ymm& b) {
    return ymm(_mm256_cmp_ps(a.X, b.X, _CMP_GE_OQ));
}

inline ymm operator <= (const ymm& a, const ymm& b) {
    return ymm(_mm256_cmp_ps(a.X, b.X, _CMP_LE_OQ));
}

inline ymm operator== (const ymm& a, const ymm& b) {
    return ymm(_mm256_cmp_ps(a.X, b.X, _CMP_EQ_OQ));
}

inline ymm operator != (const ymm& a, const ymm& b) {
    return ymm(_mm256_cmp_ps(a.X, b.X, _CMP_NEQ_OQ));
}



class ymmi8 {
  public:
    __m256i X;
    ymmi8(){}
    ymmi8(std::int8_t* p) {
        this->X = _mm256_lddqu_si256((__m256i*)p);
    }
    ymmi8(std::int8_t a, std::int8_t b, std::int8_t c, std::int8_t d,
          std::int8_t e, std::int8_t f, std::int8_t g, std::int8_t h,
          std::int8_t i, std::int8_t j, std::int8_t k, std::int8_t l,
          std::int8_t m, std::int8_t n, std::int8_t o, std::int8_t p,
          std::int8_t q, std::int8_t r, std::int8_t s, std::int8_t t,
          std::int8_t u, std::int8_t v, std::int8_t w, std::int8_t x,
          std::int8_t y, std::int8_t z, std::int8_t aa, std::int8_t bb,
          std::int8_t cc, std::int8_t dd, std::int8_t ee, std::int8_t ff )
    {
        this->X = _mm256_set_epi8(a,b,c,d,e,f,g,h,i,j,k, l, m, n, o, p,
                                  q,r,s,t,u,v,w,x,y,z,aa,bb,cc,dd,ee,ff);
    }

    ymmi8( std::int8_t a ) {
        this->X = _mm256_set1_epi8(a);
    }

    ymmi8(__m256i x) {
        this->X=x;
    }

    xmmi8 toxmm() const {
        return xmmi8(_mm256_castsi256_si128(this->X));
    }

    operator ymmi8() const;
    operator ymmi16() const;
    operator ymmi32() const;
    operator ymmi64() const;

    operator __m256i() const {
        return this->X;
    }

    static ymmi8 zeros(){
        return ymmi8( _mm256_setzero_si256() );
    }

    static ymmi8 setall(){
        return ymmi8( _mm256_cmpeq_epi8(
            _mm256_setzero_si256(),
            _mm256_setzero_si256()
        ) );
    }

    static ymmi8 ones();

    void store(std::int8_t* p) const {
        _mm256_storeu_si256((__m256i*)p, this->X);
    }

    ymmi8 shuffle(const ymmi8& mask){
        return ymmi8( _mm256_shuffle_epi8(this->X, mask.X ) );
    }
};


class ymmu8{
  public:
    __m256i X;
    ymmu8(){}
    ymmu8(std::uint8_t* p)  {
        this->X = _mm256_lddqu_si256((__m256i*)p);
    }
    ymmu8(std::uint8_t a, std::uint8_t b, std::uint8_t c, std::uint8_t d,
          std::uint8_t e, std::uint8_t f, std::uint8_t g, std::uint8_t h,
          std::uint8_t i, std::uint8_t j, std::uint8_t k, std::uint8_t l,
          std::uint8_t m, std::uint8_t n, std::uint8_t o, std::uint8_t p,
          std::uint8_t q, std::uint8_t r, std::uint8_t s, std::uint8_t t,
          std::uint8_t u, std::uint8_t v, std::uint8_t w, std::uint8_t x,
          std::uint8_t y, std::uint8_t z, std::uint8_t aa, std::uint8_t bb,
          std::uint8_t cc, std::uint8_t dd, std::uint8_t ee, std::uint8_t ff )
    {
        this->X = _mm256_set_epi8(a,b,c,d,e,f,g,h,i,j,k, l, m, n, o, p,
                                  q,r,s,t,u,v,w,x,y,z,aa,bb,cc,dd,ee,ff);
    }

    ymmu8( std::uint8_t a ) {
        this->X = _mm256_set1_epi8(a);
    }

    ymmu8(__m256i x) {
        this->X=x;
    }

    xmmu8 toxmm() const {
        return xmmu8(_mm256_castsi256_si128(this->X));
    }

    operator ymmi8() const;
    operator ymmi16() const;
    operator ymmi32() const;
    operator ymmi64() const;

    operator __m256i() const {
        return this->X;
    }

    static ymmu8 zeros(){
        return ymmu8( _mm256_setzero_si256() );
    }

    static ymmu8 setall(){
        return ymmu8( _mm256_cmpeq_epi8(
            _mm256_setzero_si256(),
            _mm256_setzero_si256()
        ) );
    }

    static ymmu8 ones();

    void store(std::uint8_t* p) const {
        _mm256_storeu_si256((__m256i*)p, this->X);
    }

    ymmu8 shuffle(const ymmi8& mask){
        return ymmu8( _mm256_shuffle_epi8(this->X, mask.X ) );
    }
};



class ymmi16 {
  public:
    __m256i X;
    ymmi16(){}
    ymmi16(std::int16_t* p)  {
        this->X = _mm256_lddqu_si256((__m256i*)p);
    }
    ymmi16(std::int16_t a, std::int16_t b, std::int16_t c, std::int16_t d,
           std::int16_t e, std::int16_t f, std::int16_t g, std::int16_t h,
           std::int16_t i, std::int16_t j, std::int16_t k, std::int16_t l,
           std::int16_t m, std::int16_t n, std::int16_t o, std::int16_t p)
    {
        this->X = _mm256_set_epi16(a,b,c,d,e,f,g,h,i,j,k, l, m, n, o, p);
    }

    ymmi16( std::int16_t a ) {
        this->X = _mm256_set1_epi16(a);
    }

    ymmi16(__m256i x) {
        this->X=x;
    }

    xmmi16 toxmm() const {
        return xmmi16(_mm256_castsi256_si128(this->X));
    }


    static ymmi16 zeros(){
        return ymmi16( _mm256_setzero_si256() );
    }

    static ymmi16 setall(){
        return ymmi16( _mm256_cmpeq_epi16(
            _mm256_setzero_si256(),
            _mm256_setzero_si256()
        ) );
    }

    static ymmi16 ones();

    operator ymmi8()  const;
    operator ymmi32() const;
    operator ymmi64() const;

    operator __m256i() const {
        return this->X;
    }

    void store(std::int16_t* p) const {
        _mm256_storeu_si256((__m256i*)p, this->X);
    }
    ymmi16 shuffle(const ymmi8& mask){
        return ymmi16( _mm256_shuffle_epi8(this->X, mask.X ) );
    }

};


class ymmi32  {
  public:
    __m256i X;
    ymmi32(){}
    ymmi32(std::int32_t* p)  {
        this->X = _mm256_lddqu_si256((__m256i*)p);
    }
    ymmi32(std::int32_t a, std::int32_t b, std::int32_t c, std::int32_t d,
           std::int32_t e, std::int32_t f, std::int32_t g, std::int32_t h)
    {
        this->X = _mm256_set_epi32(a,b,c,d,e,f,g,h);
    }

    ymmi32( std::int32_t a ) {
        this->X = _mm256_set1_epi32(a);
    }

    ymmi32(__m256i x) {
        this->X=x;
    }

    static ymmi32 zeros(){
        return ymmi32( _mm256_setzero_si256() );
    }

    static ymmi32 setall(){
        return ymmi32( _mm256_cmpeq_epi32(
            _mm256_setzero_si256(),
            _mm256_setzero_si256()
        ) );
    }

    xmmi32 toxmm() const {
        return xmmi32(_mm256_castsi256_si128(this->X));
    }
    xmmi16 toxmmi16() const{
        return xmmi16(_mm256_castsi256_si128(this->X));
    }


    static ymmi32 ones();

    operator ymmi8() const ;
    operator ymmi16()const ;
    operator ymmi64()const ;
    operator ymm() const;

    operator __m256i() const {
        return this->X;
    }

    void store(std::int32_t* p) const {
        _mm256_storeu_si256((__m256i*)p, this->X);
    }
    ymmi32 shuffle(const ymmi8& mask){
        return ymmi32( _mm256_shuffle_epi8(this->X, mask.X ) );
    }

    ymmi32 permute( const ymmi32& mask ){
        return ymmi32( _mm256_permutevar8x32_epi32(this->X, mask.X) );
    }


};


class ymmi64  {
  public:
    __m256i X;
    ymmi64(){}
    ymmi64(std::int64_t* p) {
        this->X = _mm256_lddqu_si256((__m256i*)p);
    }
    ymmi64(std::int64_t a, std::int64_t b, std::int64_t c, std::int64_t d)
    {
        this->X = _mm256_set_epi64x(a,b,c,d);
    }

    ymmi64( std::int16_t a ) {
        this->X = _mm256_set1_epi64x(a);
    }

    ymmi64(__m256i x) {
        this->X=x;
    }

    xmmi64 toxmm() const {
        return xmmi64(_mm256_castsi256_si128(this->X));
    }


    static ymmi64 zeros(){
        return ymmi64( _mm256_setzero_si256() );
    }

    static ymmi64 setall(){
        return ymmi64( _mm256_cmpeq_epi64(
            _mm256_setzero_si256(),
            _mm256_setzero_si256()
        ) );
    }

    static ymmi64 ones();

    operator ymmi8()  const;
    operator ymmi16() const;
    operator ymmi32() const;

    operator __m256i() const {
        return this->X;
    }

    void store(std::int64_t* p) const {
        _mm256_storeu_si256((__m256i*)p,this->X);
    }
    ymmi64 shuffle(const ymmi8& mask){
        return ymmi64( _mm256_shuffle_epi8(this->X, mask.X ) );
    }

};

inline ymmi8 operator+(const ymmi8& a, const ymmi8& b){
    return _mm256_add_epi8(a.X,b.X);
}

inline ymmi8 operator-(const ymmi8& a, const ymmi8& b){
    return _mm256_sub_epi8(a.X,b.X);
}

inline ymmi8 operator|(const ymmi8& a, const ymmi8& b){
    return _mm256_or_si256(a.X,b.X);
}

inline ymmi8 operator&(const ymmi8& a, const ymmi8& b){
    return _mm256_and_si256(a.X,b.X);
}

inline ymmi8 operator==(const ymmi8& a, const ymmi8& b){
    return _mm256_cmpeq_epi8(a.X,b.X);
}

inline ymmi8 operator>(const ymmi8& a, const ymmi8& b){
    return _mm256_cmpgt_epi8(a.X,b.X);
}

inline ymmi8 operator<(const ymmi8& a, const ymmi8& b){
    return b>a;
}

inline ymmi8 operator>=(const ymmi8& a, const ymmi8& b){
    return (a>b)|(a==b);
}

inline ymmi8 operator<=(const ymmi8& a, const ymmi8& b){
    return (a<b)|(a==b);
}

inline ymmi8 operator!=(const ymmi8& a, const ymmi8& b){
    return (a>b)|(a<b);
}

inline ymmi8 max(const ymmi8& a, const ymmi8& b){
    return _mm256_max_epi8(a.X,b.X);
}

inline ymmi8 min(const ymmi8& a, const ymmi8& b){
    return _mm256_min_epi8(a.X,b.X);
}




inline ymmu8 max(const ymmu8& a, const ymmu8& b){
    return _mm256_max_epu8(a.X,b.X);
}

inline ymmu8 min(const ymmu8& a, const ymmu8& b){
    return _mm256_min_epu8(a.X,b.X);
}

inline ymmu8 operator+(const ymmu8& a, const ymmu8& b){
    return _mm256_add_epi8(a.X,b.X);
}

inline ymmu8 operator-(const ymmu8& a, const ymmu8& b){
    return _mm256_sub_epi8(a.X,b.X);
}

inline ymmu8 operator|(const ymmu8& a, const ymmu8& b){
    return _mm256_or_si256(a.X,b.X);
}

inline ymmu8 operator&(const ymmu8& a, const ymmu8& b){
    return _mm256_and_si256(a.X,b.X);
}

inline ymmu8 operator==(const ymmu8& a, const ymmu8& b){
    return _mm256_cmpeq_epi8(a.X,b.X);
}

inline ymmu8 operator>=(const ymmu8& a, const ymmu8& b){
    return a == max(a,b);
}

inline ymmu8 operator>(const ymmu8& a, const ymmu8& b){
    return ymmu8( _mm256_andnot_si256( a==b, a>=b ) );
}

inline ymmu8 operator<(const ymmu8& a, const ymmu8& b){
    return b>a;
}

inline ymmu8 operator<=(const ymmu8& a, const ymmu8& b){
    return (a<b)|(a==b);
}

inline ymmu8 operator!=(const ymmu8& a, const ymmu8& b){
    return (a>b)|(a<b);
}

//bit shift
inline ymmu8 operator>>(const ymmu8& y, unsigned count)
{
    auto v = _mm256_srli_epi16(y.X,count);
    __m256i mask = _mm256_set1_epi8(0xff>>count);
    return ymmu8( _mm256_and_si256( v,mask ) );
}



inline ymmi16 operator+(const ymmi16& a, const ymmi16& b){
    return _mm256_add_epi16(a.X,b.X);
}

inline ymmi16 operator-(const ymmi16& a, const ymmi16& b){
    return _mm256_sub_epi16(a.X,b.X);
}

inline ymmi16 operator|(const ymmi16& a, const ymmi16& b){
    return _mm256_or_si256(a.X,b.X);
}

inline ymmi16 operator&(const ymmi16& a, const ymmi16& b){
    return _mm256_and_si256(a.X,b.X);
}


inline ymmi16 operator==(const ymmi16& a, const ymmi16& b){
    return _mm256_cmpeq_epi16(a.X,b.X);
}

inline ymmi16 operator>(const ymmi16& a, const ymmi16& b){
    return _mm256_cmpgt_epi16(a.X,b.X);
}

inline ymmi16 operator<(const ymmi16& a, const ymmi16& b){
    return _mm256_cmpgt_epi16(b.X,a.X);
}

inline ymmi16 operator>=(const ymmi16& a, const ymmi16& b){
    return (a>b)|(a==b);
}

inline ymmi16 operator<=(const ymmi16& a, const ymmi16& b){
    return (a<b)|(a==b);
}

inline ymmi16 operator!=(const ymmi16& a, const ymmi16& b){
    return (a>b)|(a<b);
}

inline ymmi16 max(const ymmi16& a, const ymmi16& b){
    return _mm256_max_epi16(a.X,b.X);
}

inline ymmi16 min(const ymmi16& a, const ymmi16& b){
    return _mm256_min_epi16(a.X,b.X);
}


inline ymmi32 operator+(const ymmi32& a, const ymmi32& b){
    return _mm256_add_epi32(a.X,b.X);
}

inline ymmi32 operator-(const ymmi32& a, const ymmi32& b){
    return _mm256_sub_epi32(a.X,b.X);
}

inline ymmi32 operator|(const ymmi32& a, const ymmi32& b){
    return _mm256_or_si256(a.X,b.X);
}


inline ymmi32 operator&(const ymmi32& a, const ymmi32& b){
    return _mm256_and_si256(a.X,b.X);
}

inline ymmi32 operator==(const ymmi32& a, const ymmi32& b){
    return _mm256_cmpeq_epi32(a.X,b.X);
}

inline ymmi32 operator>(const ymmi32& a, const ymmi32& b){
    return _mm256_cmpgt_epi32(a.X,b.X);
}

inline ymmi32 operator<(const ymmi32& a, const ymmi32& b){
    return _mm256_cmpgt_epi32(b.X,a.X);
}

inline ymmi32 operator>=(const ymmi32& a, const ymmi32& b){
    return (a>b)|(a==b);
}

inline ymmi32 operator<=(const ymmi32& a, const ymmi32& b){
    return (a<b)|(a==b);
}

inline ymmi32 operator!=(const ymmi32& a, const ymmi32& b){
    return (a>b)|(a<b);
}

inline ymmi32 max(const ymmi32& a, const ymmi32& b){
    return _mm256_max_epi32(a.X,b.X);
}

inline ymmi32 min(const ymmi32& a, const ymmi32& b){
    return _mm256_min_epi32(a.X,b.X);
}




inline ymmi64 operator+(const ymmi64& a, const ymmi64& b){
    return _mm256_add_epi64(a.X,b.X);
}

inline ymmi64 operator-(const ymmi64& a, const ymmi64& b){
    return _mm256_sub_epi64(a.X,b.X);
}

inline ymmi64 operator|(const ymmi64& a, const ymmi64& b){
    return _mm256_or_si256(a.X,b.X);
}

inline ymmi64 operator&(const ymmi64& a, const ymmi64& b){
    return _mm256_and_si256(a.X,b.X);
}

inline ymmi64 operator==(const ymmi64& a, const ymmi64& b){
    return _mm256_cmpeq_epi64(a.X,b.X);
}

inline ymmi64 operator>(const ymmi64& a, const ymmi64& b){
    return _mm256_cmpgt_epi64(a.X,b.X);
}

inline ymmi64 operator<(const ymmi64& a, const ymmi64& b){
    return _mm256_cmpgt_epi64(b.X,a.X);
}

inline ymmi64 operator>=(const ymmi64& a, const ymmi64& b){
    return (a>b)|(a==b);
}

inline ymmi64 operator<=(const ymmi64& a, const ymmi64& b){
    return (a<b)|(a==b);
}

inline ymmi64 operator!=(const ymmi64& a, const ymmi64& b){
    return (a>b)|(a<b);
}

inline ymmi64 max(const ymmi64& a, const ymmi64& b){
    return _mm256_max_epi64(a.X,b.X);
}

inline ymmi64 min(const ymmi64& a, const ymmi64& b){
    return _mm256_min_epi64(a.X,b.X);
}



inline ymmi8 blend(const ymmi8& a, const ymmi8& b, const ymmi8& c){
    return ymmi8( _mm256_blendv_epi8(a.X,b.X,c.X) );
}

inline ymmi16 blend(const ymmi16& a, const ymmi16& b, const ymmi8& c){
    return ymmi16( _mm256_blendv_epi8(a.X,b.X,c.X) );
}

inline ymmi32 blend(const ymmi32& a, const ymmi32& b, const ymmi8& c){
    return ymmi32( _mm256_blendv_epi8(a.X,b.X,c.X) );
}

inline ymmi64 blend(const ymmi64& a, const ymmi64& b, const ymmi8& c){
    return ymmi64( _mm256_blendv_epi8(a.X,b.X,c.X) );
}



inline ymmu8::operator ymmi8() const {
    return ymmi8(this->X);
}
inline ymmi16::operator ymmi8() const {
    return ymmi8(this->X);
}
inline ymmi32::operator ymmi8() const {
    return ymmi8(this->X);
}
inline ymmi64::operator ymmi8() const {
    return ymmi8(this->X);
}

inline ymmi8::operator ymmi16() const {
    return ymmi16(this->X);
}
inline ymmu8::operator ymmi16() const {
    return ymmi16(this->X);
}
inline ymmi32::operator ymmi16() const {
    return ymmi16(this->X);
}
inline ymmi64::operator ymmi16() const {
    return ymmi16(this->X);
}


inline ymmi8::operator ymmi32() const {
    return ymmi32(this->X);
}
inline ymmu8::operator ymmi32() const {
    return ymmi32(this->X);
}
inline ymmi16::operator ymmi32() const {
    return ymmi32(this->X);
}
inline ymmi64::operator ymmi32() const {
    return ymmi32(this->X);
}


inline ymmi8::operator ymmi64() const {
    return ymmi64(this->X);
}
inline ymmu8::operator ymmi64() const {
    return ymmi64(this->X);
}inline ymmi16::operator ymmi64() const {
    return ymmi64(this->X);
}
inline ymmi32::operator ymmi64() const {
    return ymmi64(this->X);
}


inline ymm::operator ymmi32() const {
    return ymmi32(_mm256_castps_si256(this->X));
}

inline ymmi32::operator ymm() const {
    return ymm(_mm256_castsi256_ps(this->X));
}


inline ymmi8 ymmi8::ones()
{
    return zeros() - setall();
}

inline ymmu8 ymmu8::ones()
{
    return zeros() - setall();
}
inline ymmi16 ymmi16::ones()
{
    return zeros() - setall();
}
inline ymmi32 ymmi32::ones()
{
    return zeros() - setall();
}

inline ymmi64 ymmi64::ones()
{
    return zeros() - setall();
}

inline ymmi8 unpacklo( const ymmi8& a, const ymmi8& b)
{
    return _mm256_unpacklo_epi8(a,b);
}

inline ymmi8 unpackhi( const ymmi8& a, const ymmi8& b)
{
    return _mm256_unpackhi_epi8(a,b);
}

inline ymmu8 unpacklo( const ymmu8& a, const ymmu8& b)
{
    return _mm256_unpacklo_epi8(a,b);
}

inline ymmu8 unpackhi( const ymmu8& a, const ymmu8& b)
{
    return _mm256_unpackhi_epi8(a,b);
}



inline ymmi16 unpacklo( const ymmi16& a, const ymmi16& b)
{
    return _mm256_unpacklo_epi16(a,b);
}

inline ymmi16 unpackhi( const ymmi16& a, const ymmi16& b)
{
    return _mm256_unpackhi_epi16(a,b);
}


inline ymmi32 unpacklo( const ymmi32& a, const ymmi32& b)
{
    return _mm256_unpacklo_epi32(a,b);
}

inline ymmi32 unpackhi( const ymmi32& a, const ymmi32& b)
{
    return _mm256_unpackhi_epi32(a,b);
}


inline ymmi64 unpacklo( const ymmi64& a, const ymmi64& b)
{
    return _mm256_unpacklo_epi64(a,b);
}

inline ymmi64 unpackhi( const ymmi64& a, const ymmi64& b)
{
    return _mm256_unpackhi_epi64(a,b);
}

inline std::ostream& operator<<(std::ostream& o, const ymmi8& Y)
{
    std::int8_t A[32];
    Y.store(A);
    o << "[";
    for(int i=0;i<32;++i){
        if( i != 0 )
            o << " , ";
        o << int(A[i]);
    }
    o << "]";
    return o;
}


inline std::ostream& operator<<(std::ostream& o, const ymmu8& Y)
{
    std::uint8_t A[32];
    Y.store(A);
    o << "[";
    for(int i=0;i<32;++i){
        if( i != 0 )
            o << " , ";
        o << unsigned(A[i]);
    }
    o << "]";
    return o;
}


inline std::ostream& operator<<(std::ostream& o, const ymmi16& Y)
{
    std::int16_t A[32];
    Y.store(A);
    o << "[";
    for(int i=0;i<16;++i){
        if( i != 0 )
            o << " , ";
        o << A[i];
    }
    o << "]";
    return o;
}


inline std::ostream& operator<<(std::ostream& o, const ymmi32& Y)
{
    std::int32_t A[32];
    Y.store(A);
    o << "[";
    for(int i=0;i<8;++i){
        if( i != 0 )
            o << " , ";
        o << A[i];
    }
    o << "]";
    return o;
}


inline std::ostream& operator<<(std::ostream& o, const ymmi64& Y)
{
    std::int64_t A[32];
    Y.store(A);
    o << "[";
    for(int i=0;i<4;++i){
        if( i != 0 )
            o << " , ";
        o << A[i];
    }
    o << "]";
    return o;
}
