
#include "y/dft/dftn.hpp"
#include "y/utest/run.hpp"
#include "y/field/3d.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/system/rtti.hpp"

using namespace Yttrium;


namespace  {

    static const unit_t Max2D = 64;

    template <typename T>
    static inline
    void Test2D()
    {
        std::cerr << "Testing 2D with " << RTTI::Name<T>() << std::endl;
        for(unit_t nx=1;nx<=Max2D;nx*=2)
        {
            for(unit_t ny=1;ny<=Max2D;ny*=2)
            {
                Field::Format2D  L = new Field::Layout2D( Field::Coord2D(1,1), Field::Coord2D(nx,ny) );

                typedef Field::In2D< Complex<T>, Memory::Dyadic > F2D;
                F2D F("F2D",L);

                std::cerr << F << std::endl;
                for(unit_t j=F->lower.y;j<=F->upper.y;++j)
                {
                    //std::cerr << "j=" << j << std::endl;
                    for(unit_t i=F->lower.x;i<=F->upper.x;++i)
                    {
                        F[j][i].re = i;
                        F[j][i].im = j;
                    }
                    //Core::Display( std::cerr, &F[j][1].re, 2*F->width.x ) << std::endl;
                }

                const size_t  nn[] = { 0, F->width.y, F->width.x };
                T * const data = (&F[1][1].re) - 1;
                DFTN::Forward(data, nn, 2);
                DFTN::Reverse(data, nn, 2);

                //std::cerr << std::endl;
                static const T half = 0.5;
                for(unit_t j=F->lower.y;j<=F->upper.y;++j)
                {
                    //std::cerr << "j=" << j << std::endl;
                    for(unit_t i=F->lower.x;i<=F->upper.x;++i)
                    {
                        F[j][i] /= F->shift.y;
                        const unit_t re = (unit_t)( floor(F[j][i].re+half) );
                        const unit_t im = (unit_t)( floor(F[j][i].im+half) );
                        Y_ASSERT(re==i);
                        Y_ASSERT(im==j);
                    }
                    //Core::Display( std::cerr, &F[j][1].re, F->width.x*2 ) << std::endl;
                }
            }
        }
        std::cerr << std::endl;
    }

    static const unit_t Max3D = 64;

    template <typename T>
    static inline
    void Test3D()
    {
        std::cerr << "Testing 3D with " << RTTI::Name<T>() << std::endl;
        for(unit_t nx=1;nx<=Max3D;nx*=2)
        {
            for(unit_t ny=1;ny<=Max3D;ny*=2)
            {
                for(unit_t nz=1;nz<=Max3D;nz*=2)
                {
                    Field::Format3D  L = new Field::Layout3D( Field::Coord3D(1,1,1), Field::Coord3D(nx,ny,nz) );
                    typedef Field::In3D< Complex<T>, Memory::Dyadic > F3D;
                    F3D F("F3D",L);
                    std::cerr << F << std::endl;

                    for(unit_t k=F->lower.z;k<=F->upper.z;++k)
                    {
                        for(unit_t j=F->lower.y;j<=F->upper.y;++j)
                        {
                            for(unit_t i=F->lower.x;i<=F->upper.x;++i)
                            {
                                F[k][j][i].re = i+j;
                                F[k][j][i].im = j+k;
                            }
                        }
                    }

                    const size_t  nn[] = { 0, F->width.z, F->width.y, F->width.x };
                    T * const     data = (&F[1][1][1].re) - 1;

                    DFTN::Forward(data, nn, 3);
                    DFTN::Reverse(data, nn, 3);

                    static const T half = 0.5;
                    for(unit_t k=F->lower.z;k<=F->upper.z;++k)
                    {
                        for(unit_t j=F->lower.y;j<=F->upper.y;++j)
                        {
                            for(unit_t i=F->lower.x;i<=F->upper.x;++i)
                            {
                                F[k][j][i] /= F->shift.z;
                                const unit_t re = (unit_t)( floor(F[k][j][i].re+half) );
                                const unit_t im = (unit_t)( floor(F[k][j][i].im+half) );
                                Y_ASSERT(re==i+j);
                                Y_ASSERT(im==j+k);
                            }
                        }
                    }
                }
            }

        }
        std::cerr << std::endl;
    }

}

Y_UTEST(dft_nd)
{
    Test2D<float>();
    Test2D<double>();
    Test2D<long double>();

    Test3D<float>();
    Test3D<double>();
    Test3D<long double>();

}
Y_UDONE()

