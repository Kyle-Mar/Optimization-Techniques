#pragma once

#ifdef _MSC_VER
//windows
#include <intrin.h>
#include <immintrin.h>
#include <iostream>
#else
//linux, mac
#include <mmintrin.h> //mmx
#include <xmmintrin.h> //sse
#include <emmintrin.h> //sse2
#include <pmmintrin.h> //sse3
#include <tmmintrin.h> //ssse3
#include <smmintrin.h> //sse4.1
#include <nmmintrin.h> //sse4.2
#include <immintrin.h> //avx, avx2
#endif

#include <cstdint>


//xmm, 32 bit float
class xmm {
public:
    __m128 X;       //(tm)

    //Default constructor: Set it to zero
    xmm() {
        this->X = _mm_setzero_ps();
    }

    //this assumes f points to at least 4 floats
    xmm(const float f[])
    {
        this->X = _mm_loadu_ps(f);
    }

    //this assumes f points to at least 4 floats
    xmm(float f)
    {
        this->X = _mm_set1_ps(f);
    }

    xmm(float lo, float mid, float hi, float tippytop) {
        this->X = _mm_set_ps(tippytop, hi, mid, lo);
    }

    xmm(__m128 other) {
        this->X = other;
    }

    xmm(const xmm& Q) {
        this->X = Q.X;
    }

    operator float() const {
        return _mm_cvtss_f32(this->X);
    }

    static xmm zeros() {
        return xmm(_mm_setzero_ps());
    }
    void operator=(const xmm& maroscher) {
        //std::cout << "=";
        this->X = maroscher.X;
    }

    void store(float* f) const {
        //std::cout << "Store";
        _mm_storeu_ps(f, this->X);
    }
    


};

inline xmm operator+(const xmm& a, const xmm& b)
{
    return xmm(
        _mm_add_ps(a.X, b.X)
    );
}

inline xmm operator-(const xmm& a, const xmm& b) {
    return xmm(
        _mm_sub_ps(a.X, b.X)
    );
}

inline xmm operator/ (const xmm& a, const xmm& b) {
    return xmm(
        _mm_div_ps(a.X, b.X)
    );
}

inline xmm operator* (const xmm& a, const xmm& b) {
    //std::cout << "mul";
    return xmm(
        _mm_mul_ps(a.X, b.X)
    );
}

inline xmm operator== (xmm a, xmm b) {
    return _mm_cmpeq_ps(a.X, b.X);
}

/*inline xmm operator!= (xmm a, xmm b) {
    return _mm_cmpne_ps(a.X, b.X);
}*/

inline xmm operator> (xmm a, xmm b) {
    return _mm_cmpgt_ps(a.X, b.X);
}

class xmmi8;
class xmmu8;
class xmmu16;
class xmmu32;
class xmmu64;
class xmmi16;
class xmmi32;
class xmmi64;

//xmm, 8 bit integer
class xmmi8 {
public:
    __m128i X;

    //Default constructor: Set it to zero
    xmmi8() {
        this->X = _mm_setzero_si128();
    }

    //this assumes f points to at least 4 bytes
    xmmi8(const std::int8_t f[])
    {
        // was _mm_loadu_sh(f);
        this->X = _mm_loadu_epi8(f);
    }

    xmmi8(std::int8_t f)
    {
        this->X = _mm_set1_epi8(f);
    }

    xmmi8(std::int8_t a0, std::int8_t a1, std::int8_t a2, std::int8_t a3,
        std::int8_t a4, std::int8_t a5, std::int8_t a6, std::int8_t a7,
        std::int8_t a8, std::int8_t a9, std::int8_t a10, std::int8_t a11,
        std::int8_t a12, std::int8_t a13, std::int8_t a14, std::int8_t a15)
    {
        this->X = _mm_set_epi8(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);
    }

    operator xmmi16() const;
    operator xmmi32() const;
    operator xmmi64() const;

    xmmi8(__m128i other) {
        this->X = other;
    }

    xmmi8(const xmmi8& Q) {
        this->X = Q.X;
    }

    void operator=(const xmmi8& X) {
        this->X = X.X;
    }

    void store(std::int8_t* f) const {
        _mm_storeu_si128((__m128i*) f, this->X);
    }

    xmmi8 shuffle(xmmi8 mask) {
        return xmmi8(_mm_shuffle_epi8(this->X, mask.X));
    }

    xmmi8 blend(xmmi8 b, xmmi8 c) {
        return xmmi8(_mm_blendv_epi8(this->X, b.X, c.X));
    }

};

class xmmi16 {
public:
    __m128i X;
    xmmi16() {
        this->X = _mm_setzero_si128();
    }

    // sets many things to many things
    xmmi16(const std::int16_t f[]) {
        this->X = _mm_loadu_epi32(f);
    }

    //sets all to one thing
    xmmi16(std::int16_t f)
    {
        this->X = _mm_set1_epi16(f);
    }

    xmmi16(std::int16_t e7, std::int16_t e6, std::int16_t e5, std::int16_t e4,
        std::int16_t e3, std::int16_t e2, std::int16_t e1, std::int16_t e0)
    {
        this->X = _mm_set_epi16(e7, e6, e5, e4, e3, e2, e1, e0);
    }

    operator xmmi8() const;
    operator xmmi32() const;
    operator xmmi64() const;

    xmmi16(__m128i other) {
        this->X = other;
    }

    xmmi16(const xmmi16& Q) {
        this->X = Q.X;
    }

    void operator=(const xmmi16& X) {
        this->X = X.X;
    }

    void store(std::int16_t* f) const {
        _mm_storeu_si128((__m128i*) f, this->X);
    }

    void store(std::uint16_t* f) const {
        _mm_storeu_si128((__m128i*) f, this->X);
    }


    /*xmm shuffle(xmmi8 mask) {
        return xmm(_mm_castsi128_ps(
            _mm_shuffle_epi8(
                _mm_castps_si128(this->X), mask.X)
        ));
    }*/

};

class xmmi32 {
public:
    __m128i X;
    xmmi32() {
        this->X = _mm_setzero_si128();
    }

    // sets many things to many things
    xmmi32(const std::int32_t f[]) {
        this->X = _mm_loadu_epi32(f);
    }

    //sets all to one thing
    xmmi32(std::int32_t f)
    {
        this->X = _mm_set1_epi32(f);
    }

    xmmi32(std::int32_t e3, std::int32_t e2, std::int32_t e1, std::int32_t e0)
    {
        this->X = _mm_set_epi32(e3, e2, e1, e0);
    }

    operator xmmi16() const;
    operator xmmi8() const;
    operator xmmi64() const;

    xmmi32(__m128i other) {
        this->X = other;
    }

    xmmi32(const xmmi32& Q) {
        this->X = Q.X;
    }
    
    static xmmi32 zeroes() {
        return xmmi32(_mm_setzero_si128());
    }

    void operator=(const xmmi32& X) {
        this->X = X.X;
    }

    void store(std::int32_t* f) const {
        _mm_storeu_si128((__m128i*) f, this->X);
    }

    
};

class xmmi64 {
public:
    __m128i X;
    xmmi64() {
        this->X = _mm_setzero_si128();
    }

    // sets many things to many things
    xmmi64(const std::int64_t f[]) {
        this->X = _mm_loadu_epi64(f);
    }

    //sets all to one thing
    xmmi64(std::int64_t f)
    {
        this->X = _mm_set1_epi64x(f);
    }

    xmmi64(std::int64_t e1, std::int64_t e0)
    {
        this->X = _mm_set_epi64x(e1, e0);
    }

    operator xmmi16() const;
    operator xmmi32() const;
    operator xmmi8() const;

    xmmi64(__m128i other) {
        this->X = other;
    }

    xmmi64(const xmmi64& Q) {
        this->X = Q.X;
    }

    void operator=(const xmmi64& X) {
        this->X = X.X;
    }

    void store(std::int64_t* f) const {
        _mm_storeu_si128((__m128i*) f, this->X);
    }
};
//rest of 16, 32, 64 bit classes

class xmmu8 {
public:
    __m128i X;
    //Default constructor: Set it to zero
    xmmu8() {
        this->X = _mm_setzero_si128();
    }

    //this assumes f points to at least 4 bytes
    xmmu8(const std::uint8_t f[])
    {
        // was _mm_loadu_sh(f);
        this->X = _mm_loadu_epi8(f);
    }

    xmmu8(std::uint8_t f)
    {
        this->X = _mm_set1_epi8(f);
    }

    xmmu8(std::uint8_t a0, std::uint8_t a1, std::uint8_t a2, std::uint8_t a3,
        std::uint8_t a4, std::uint8_t a5, std::uint8_t a6, std::uint8_t a7,
        std::uint8_t a8, std::uint8_t a9, std::uint8_t a10, std::uint8_t a11,
        std::uint8_t a12, std::uint8_t a13, std::uint8_t a14, std::uint8_t a15)
    {
        this->X = _mm_set_epi8(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);
    }



    operator xmmu16() const;
    operator xmmu32() const;
    operator xmmu64() const;
    operator __m128i() const;

    xmmu8(__m128i other) {
        this->X = other;
    }

    xmmu8(const xmmu8& Q) {
        this->X = Q.X;
    }

    void operator=(const xmmi8& X) {
        this->X = X.X;
    }

    void store(std::uint8_t* f) const {
        _mm_storeu_si128((__m128i*) f, this->X);
    }
};

class xmmu16 {
public:
    __m128i X;
    xmmu16() {
        this->X = _mm_setzero_si128();
    }

    // sets many things to many things
    xmmu16(const std::uint16_t f[]) {
        this->X = _mm_loadu_epi32(f);
    }

    //sets all to one thing
    xmmu16(std::uint16_t f)
    {
        this->X = _mm_set1_epi16(f);
    }

    xmmu16(std::uint16_t e7, std::uint16_t e6, std::uint16_t e5, std::uint16_t e4,
        std::uint16_t e3, std::uint16_t e2, std::uint16_t e1, std::uint16_t e0)
    {
        this->X = _mm_set_epi16(e7, e6, e5, e4, e3, e2, e1, e0);
    }

    operator xmmu8() const;
    operator xmmu32() const;
    operator xmmu64() const;

    xmmu16(__m128i other) {
        this->X = other;
    }

    xmmu16(const xmmu16& Q) {
        this->X = Q.X;
    }

    void operator=(const xmmu16& X) {
        this->X = X.X;
    }

    void store(std::uint16_t* f) const {
        _mm_storeu_si128((__m128i*) f, this->X);
    }
};

class xmmu32 {
public:
    __m128i X;
    xmmu32() {
        this->X = _mm_setzero_si128();
    }

    // sets many things to many things
    xmmu32(const std::uint32_t f[]) {
        this->X = _mm_loadu_epi32(f);
    }

    //sets all to one thing
    xmmu32(std::uint32_t f)
    {
        this->X = _mm_set1_epi32(f);
    }

    xmmu32(std::uint32_t e3, std::uint32_t e2, std::uint32_t e1, std::uint32_t e0)
    {
        this->X = _mm_set_epi32(e3, e2, e1, e0);
    }

    operator xmmu16() const;
    operator xmmu8() const;
    operator xmmu64() const;

    xmmu32(__m128i other) {
        this->X = other;
    }

    xmmu32(const xmmu32& Q) {
        this->X = Q.X;
    }

    void operator=(const xmmu32& X) {
        this->X = X.X;
    }

    void store(std::uint32_t* f) const {
        _mm_storeu_si128((__m128i*) f, this->X);
    }
};

class xmmu64 {
public:
    __m128i X;
    xmmu64() {
        this->X = _mm_setzero_si128();
    }

    // sets many things to many things
    xmmu64(const std::uint64_t f[]) {
        this->X = _mm_loadu_epi64(f);
    }

    //sets all to one thing
    xmmu64(std::uint64_t f)
    {
        this->X = _mm_set1_epi64x(f);
    }

    xmmu64(std::uint64_t e1, std::uint64_t e0)
    {
        this->X = _mm_set_epi64x(e1, e0);
    }

    operator xmmu16() const;
    operator xmmu32() const;
    operator xmmu8() const;

    xmmu64(__m128i other) {
        this->X = other;
    }

    xmmu64(const xmmu64& Q) {
        this->X = Q.X;
    }

    void operator=(const xmmu64& X) {
        this->X = X.X;
    }

    void store(std::uint64_t* f) const {
        _mm_storeu_si128((__m128i*) f, this->X);
    }
};


#pragma region cast signed
//one example; rest are similar
inline xmmi8::operator xmmi32() const {
    return xmmi8(this->X);
}
inline xmmi8::operator xmmi16() const {
    return xmmi8(this->X);
}
inline xmmi8::operator xmmi64() const {
    return xmmi8(this->X);
}

inline xmmi16::operator xmmi8() const {
    return xmmi16(this->X);
}
inline xmmi16::operator xmmi32() const {
    return xmmi16(this->X);
}
inline xmmi16::operator xmmi64() const {
    return xmmi64(this->X);
}

inline xmmi32::operator xmmi8() const {
    return xmmi32(this->X);
}
inline xmmi32::operator xmmi16() const {
    return xmmi16(this->X);
}
inline xmmi32::operator xmmi64() const {
    return xmmi32(this->X);
}

inline xmmi64::operator xmmi8() const {
    return xmmi64(this->X);
}
inline xmmi64::operator xmmi16() const {
    return xmmi64(this->X);
}
inline xmmi64::operator xmmi32() const {
    return xmmi64(this->X);
}
#pragma endregion

#pragma region cast unsigned
inline xmmu8::operator xmmu32() const {
    return xmmu32(this->X);
}
inline xmmu8::operator xmmu16() const {
    return xmmu8(this->X);
}
inline xmmu8::operator xmmu64() const {
    return xmmu8(this->X);
}
inline xmmu8::operator __m128i() const
{
    return this->X;
}

inline xmmu16::operator xmmu8() const {
    return xmmu16(this->X);
}
inline xmmu16::operator xmmu32() const {
    return xmmu16(this->X);
}
inline xmmu16::operator xmmu64() const {
    return xmmu64(this->X);
}

inline xmmu32::operator xmmu8() const {
    return xmmu8(this->X);
}
inline xmmu32::operator xmmu16() const {
    return xmmu32(this->X);
}
inline xmmu32::operator xmmu64() const {
    return xmmu32(this->X);
}

inline xmmu64::operator xmmu8() const {
    return xmmu64(this->X);
}
inline xmmu64::operator xmmu16() const {
    return xmmu64(this->X);
}
inline xmmu64::operator xmmu32() const {
    return xmmu64(this->X);
}
#pragma endregion


inline  xmmi8 operator+(const xmmi8& a, const xmmi8& b)
{
    return xmmi8(
        _mm_add_epi8(a.X, b.X)
    );
}
inline  xmmi8 operator-(const xmmi8& a, const xmmi8& b)
{
    return xmmi8(
        _mm_sub_epi8(a.X, b.X)
    );
}
inline xmmi8 operator|(xmmi8 a, xmmi8 b) {
    return xmmi8(_mm_or_si128(a.X, b.X));
}

inline xmmi8 operator==(xmmi8 a, xmmi8 b) {
    return xmmi8(_mm_cmpeq_epi8(a.X, b.X));
}
inline xmmi8 operator>(xmmi8 a, xmmi8 b) {
    return xmmi8(_mm_cmpgt_epi8(a.X, b.X));
}
inline xmmi8 operator<(xmmi8 a, xmmi8 b) {
    return xmmi8(_mm_cmpgt_epi8(b.X, a.X));
}
inline xmmi8 operator>=(xmmi8 a, xmmi8 b) {
    return (a > b) | (a == b);
}
inline xmmi8 operator<=(xmmi8 a, xmmi8 b) {
    return (a < b) | (a == b);
}
inline xmmi8 operator!=(xmmi8 a, xmmi8 b) {
    return (a > b) | (a < b);
}

inline xmmi16 operator+(const xmmi16& a, const xmmi16& b)
{
    return xmmi16(
        _mm_add_epi16(a.X, b.X)
    );
}
inline xmmi16 operator-(const xmmi16& a, const xmmi16& b)
{
    return xmmi16(
        _mm_sub_epi16(a.X, b.X)
    );
}

inline xmmi16 operator==(xmmi16 a, xmmi16 b) {
    return xmmi16(_mm_cmpeq_epi16(a.X, b.X));
}

inline  xmmi32 operator+(const xmmi32& a, const xmmi32& b)
{
    return xmmi32(
        _mm_add_epi32(a.X, b.X)
    );
}

inline xmmi32 operator*(const xmmi32& a, const xmmi32& b) {
    return xmmi32(_mm_mullo_epi32(a.X, b.X));
}

inline  xmmi32 operator-(const xmmi32& a, const xmmi32& b)
{
    return xmmi32(
        _mm_sub_epi32(a.X, b.X)
    );
}

inline xmmi32 operator==(xmmi32 a, xmmi32 b) {
    return xmmi32(_mm_cmpeq_epi32(a.X, b.X));
}

inline  xmmi64 operator+(const xmmi64& a, const xmmi64& b)
{
    return xmmi64(
        _mm_add_epi64(a.X, b.X)
    );
}
inline  xmmi64 operator-(const xmmi64& a, const xmmi64& b)
{
    return xmmi64(
        _mm_sub_epi64(a.X, b.X)
    );
}
inline xmmi64 operator>(xmmi64 a, const xmmi64 b) {
    return xmmi64(_mm_cmpgt_epi64(a.X, b.X));
}


inline  xmmu8 operator+(const xmmu8& a, const xmmu8& b)
{
    return xmmu8(
        _mm_add_epi8(a.X, b.X)
    );
}
inline  xmmu8 operator-(const xmmu8& a, const xmmu8& b)
{
    return xmmu8(
        _mm_sub_epi8(a.X, b.X)
    );
}
inline xmmu8 operator==(xmmu8 a, xmmu8 b) {
    return xmmu8(_mm_cmpeq_epi8(a.X, b.X));
}
inline xmmu8 max(xmmu8 a, xmmu8 b) {
    return xmmu8(_mm_max_epu8(a.X, b.X));
}
inline xmmu8 operator>=(xmmu8 a,  xmmu8 b) {
    return a == max(a, b);
}
inline xmmu8 operator<=(xmmu8 a, xmmu8 b) {
    return b == max(a, b);
}
inline xmmu8 operator> (xmmu8 a, xmmu8 b) {
    return xmmu8(
        _mm_andnot_si128(
            a==b,
            a>=b
        )
    );
}

inline xmmu8 operator< (xmmu8 a, xmmu8 b) {
    return b > a;
}

inline xmmu8 operator| (xmmu8 a, xmmu8 b) {
    return xmmu8(_mm_or_si128(a, b));
}
inline xmmu8 operator& (xmmu8 a, xmmu8 b) {
    return xmmu8(_mm_and_si128(a, b));
}


inline xmmi16 abs_dif(xmmu8 a, xmmu8 b) {
    return xmmi16(_mm_sad_epu8(a.X, b.X));
}

inline xmmu16 operator+(const xmmu16& a, const xmmu16& b)
{
    return xmmu16(
        _mm_add_epi16(a.X, b.X)
    );
}

inline xmmu16 operator-(const xmmu16& a, const xmmu16& b)
{
    return xmmu16(
        _mm_sub_epi16(a.X, b.X)
    );
}

inline xmmu32 operator+(const xmmu32& a, const xmmu32& b)
{
    return xmmu32(
        _mm_add_epi32(a.X, b.X)
    );
}

inline xmmu32 operator-(const xmmu32& a, const xmmu32& b)
{
    return xmmu32(
        _mm_sub_epi32(a.X, b.X)
    );
}

inline xmmu32 operator==(const xmmu32& a, const xmmu32& b) {
    return xmmu32(
        _mm_cmpeq_epi32(a.X, b.X)
    );
}


inline xmmu64 operator+(const xmmu64& a, const xmmu64& b)
{
    return xmmu64(
        _mm_add_epi64(a.X, b.X)
    );
}

inline xmmu64 operator-(const xmmu64& a, const xmmu64& b)
{
    return xmmu64(
        _mm_sub_epi64(a.X, b.X)
    );
}
inline xmmu64 operator>(xmmu64 a, const xmmu64 b) {
    return xmmu64(_mm_cmpgt_epi64(a.X, b.X));
}

inline xmmi64 operator==(xmmi64 a, xmmi64 b) {
    return xmmi64(_mm_cmpeq_epi64(a.X, b.X));
}

inline xmmu8 string_in_range(const xmmu8& a, const xmmu8& b, int length_a, int length_b) {
   return xmmu8(_mm_cmpestrm(a, length_a, b, length_b, _SIDD_UBYTE_OPS | _SIDD_CMP_RANGES | _SIDD_UNIT_MASK));
}

inline xmmu8 blendv(const xmmu8& orig, const xmmu8& repl, const xmmu32& mask) {
    return xmmu8(_mm_blendv_epi8(orig.X, repl.X, mask.X));
}