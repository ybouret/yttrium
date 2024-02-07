
#include "../main.hpp"
#include "y/utest/run.hpp"

#include "y/mkl/antelope/add.hpp"
#include "y/text/justify.hpp"
#include "y/sequence/vector.hpp"

#include "y/data/list/cxx.hpp"
#include "y/data/pool/cxx.hpp"


using namespace Yttrium;
using namespace MKL;

namespace Yttrium
{
    namespace MKL
    {
        namespace Antelope
        {
            template <typename T>
            class AddNode : public Object, public Add<T>
            {
            public:
                typedef CxxListOf<AddNode> List;
                typedef CxxPoolOf<AddNode> Pool;

                inline explicit AddNode() : next(0), prev(0)
                {
                }

                inline virtual ~AddNode() noexcept
                {
                }

                AddNode *next;
                AddNode *prev;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(AddNode);
            };

            template <typename T>
            class Caddy : public AddNode<T>::List
            {
            public:
                typedef typename AddNode<T>::List ListType;
                typedef
                explicit Caddy()
                {
                }

                virtual ~Caddy() noexcept
                {
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Caddy);

            };
        }
    }
}


template <typename T>
static inline void ShowUnit( const char *name, Random::Bits &ran )
{
    std::cerr << '[' << Justify(name,32,Justify::Center) << ']' << std::endl;

    MKL::Antelope::Add<T> xadd;
    xadd.free();

    std::cerr << xadd.sum();

    Vector<T> raw;
    for(size_t i=1+ran.leq(10);i>0;--i)
    {
        const T tmp = Bring<T>::Get(ran);
        std::cerr << "+(" << tmp << ")";
        xadd << tmp;
        raw  << tmp;
    }
    
    const T xsum = xadd.sum();
    T rsum(0);
    for(size_t i=1;i<=raw.size();++i) rsum += raw[i];
    std::cerr << " = " << xsum << "/" << rsum << std::endl;

    if(xadd.Exact)
    {
        Y_CHECK(xsum==rsum);
    }
    else
    {
        const T delta = xsum-rsum;
        std::cerr << "delta=" << Fabs<T>::Of(delta) << std::endl;
    }

    {
        Vector<T> someData;
        for(size_t n=0;n<=5;++n)
        {
            someData.free();
            for(size_t i=n;i>0;--i)
            {
                someData << Bring<T>::Get(ran);
            }
            std::cerr << "average(" << someData << ")=" << xadd.average( someData ) << std::endl;
        }
    }

}

#define Y_SHOW_UNIT(CLASS) ShowUnit<CLASS>( #CLASS, ran)



Y_UTEST(mkl_xadd)
{
    Random::Rand ran;

    Y_SHOW_UNIT(float);
    Y_SHOW_UNIT(double);
    Y_SHOW_UNIT(long double);

    Y_SHOW_UNIT(Complex<float>);
    Y_SHOW_UNIT(Complex<double>);
    Y_SHOW_UNIT(Complex<long double>);


    Y_SHOW_UNIT(XReal<float>);
    Y_SHOW_UNIT(XReal<double>);
    Y_SHOW_UNIT(XReal<long double>);
    Y_SHOW_UNIT(Complex< XReal<float> >);
    Y_SHOW_UNIT(Complex< XReal<double> >);
    Y_SHOW_UNIT(Complex< XReal<long double> >);

    Y_SHOW_UNIT(apn);
    Y_SHOW_UNIT(apz);
    Y_SHOW_UNIT(apq);


    {
        Antelope::Add<double> xadd;
        std::cerr << std::endl;
        V3D<double> v(-3,1,-2);
        std::cerr << "|" << v << "| = " << v.norm() << " / " << xadd.normOf(v) << std::endl;

        xadd.normalize(v);
        std::cerr << "|" << v << "| = " << v.norm() << " / " << xadd.normOf(v) << std::endl;
    }

    Y_SIZEOF(Antelope::AddNode<double>);
    Y_SIZEOF(Antelope::AddNode< XReal<long double> >);
    Y_SIZEOF(Antelope::AddNode< apq >);
    Y_SIZEOF(Antelope::AddNode< apz >);


}
Y_UDONE()
