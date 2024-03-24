#include "xmm.h"
#include <cstdlib>
#include <cassert>
#include <iostream>

template<typename ST,typename XT>
void testi(int n, ST dummy0, XT X)
{
    ST xp[16] = {
        3,1,4,1,5,9,2,6,5,3,5,8,9,7,9,3
    };
    ST tmp[16];
    X.store(tmp);
    for(int i=0;i<n;++i){
        assert( xp[n-i-1] == tmp[i] );
    }
    
    ST I[16] = {
        (ST)rand(), (ST)rand(), (ST)rand(), (ST)rand(),
        (ST)rand(), (ST)rand(), (ST)rand(), (ST)rand(),
        (ST)rand(), (ST)rand(), (ST)rand(), (ST)rand(),
        (ST)rand(), (ST)rand(), (ST)rand(), (ST)rand()
    };
    ST J[16] = {
        (ST)rand(), (ST)rand(), (ST)rand(), (ST)rand(),
        (ST)rand(), (ST)rand(), (ST)rand(), (ST)rand(),
        (ST)rand(), (ST)rand(), (ST)rand(), (ST)rand(),
        (ST)rand(), (ST)rand(), (ST)rand(), (ST)rand()
    };
    ST K[16];
    XT a(I);
    XT b(J);
    
    XT c = a+b;
    c.store(K);
    for(int i=0;i<n;++i){
        assert(K[i] == ST(I[i]+J[i]));
    }
    
    c = a-b;
    c.store(K);
    for(int i=0;i<n;++i){
        assert(K[i] == ST(I[i]-J[i]));
    }
    
}

/*int main(int argc, char* argv[])
{
    srand(123);
    
    for(int i=0;i<100;++i){
        float a[4] = {
                        float(rand()*1.0/RAND_MAX),
                        float(rand()*1.0/RAND_MAX),
                        float(rand()*1.0/RAND_MAX),
                        float(rand()*1.0/RAND_MAX)
        };
        float b[4] = {
                        float(rand()*1.0/RAND_MAX),
                        float(rand()*1.0/RAND_MAX),
                        float(rand()*1.0/RAND_MAX),
                        float(rand()*1.0/RAND_MAX)
        };
        float c[4];
        xmm x1(a);
        xmm x2(b);
        xmm x3 = x1+x2;
        x3.store(c);
        for(int i=0;i<4;++i){
            assert( c[i] == a[i]+b[i]);
        }

        x3 = x1-x2;
        x3.store(c);
        for(int i=0;i<4;++i){
            assert( c[i] == a[i]-b[i]);
        }
            
        x3 = x1*x2;
        x3.store(c);
        for(int i=0;i<4;++i){
            assert( c[i] == a[i]*b[i]);
        }

        x3 = x1/x2;
        x3.store(c);
        for(int i=0;i<4;++i){
            assert( c[i] == a[i]/b[i]);
        }

        testi( 16, std::int8_t(0),      xmmi8(3,1,4,1,5,9,2,6,5,3,5,8,9,7,9,3));
        testi( 16, std::uint8_t(0),     xmmu8(3,1,4,1,5,9,2,6,5,3,5,8,9,7,9,3) );
        testi( 8,  std::int16_t(0),     xmmi16(3,1,4,1,5,9,2,6) );
        testi( 8,  std::uint16_t(0),    xmmu16(3,1,4,1,5,9,2,6) );
        testi( 4,  std::int32_t(0),     xmmi32(3,1,4,1) );
        testi( 4,  std::uint32_t(0),    xmmu32(3,1,4,1) );
        testi( 2,  std::int64_t(0),     xmmi64(3,1) );
        testi( 2,  std::uint64_t(0),    xmmu64(3,1) );
    }
    std::cout << "OK\n";
    return 0;
}*/

