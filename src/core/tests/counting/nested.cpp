#include "y/counting/nested-loop.hpp"

#include "y/container/cxx/array.hpp"

#include "y/utest/run.hpp"


using namespace Yttrium;

#if 0
namespace Yttrium
{
    typedef CxxArray<size_t> LoopArray;

    class NestedLoop : public LoopArray
    {
    public:

        static apn apCount(const size_t d,
                           const size_t n)
        {
            assert(d>=1);
            assert(n>=d);
            apn res = 1;
            for(size_t i=0;i<d;++i) {
                res *= n-i;
            }
            return res / apn::Factorial(d);
        }

        explicit NestedLoop(const size_t dims,
                            const size_t nmax) :
        LoopArray(dims,0),
        imin(dims,0),
        imax(dims,0)
        {
            assert(nmax>=dims);
            const size_t delta = nmax-dims;
            for(size_t i=1;i<=dims;++i)
            {
                Coerce(imin[i]) = i;
                Coerce(imax[i]) = i+delta;
            }
            init();
        }

        void reset() noexcept { init(); }

        virtual ~NestedLoop() noexcept {}

        bool next()
        {
            return move( size() );
        }

        bool move(const size_t k)
        {
            size_t &i = (*this)[k];
            if(i<imax[k])
            {
                ++i;
                return true;
            }
            else
            {
                if(k<=1)
                {
                    return false;
                }
                else
                {
                    if(move(k-1))
                    {
                        for(size_t j=k;j<=size();++j)
                        {
                            (*this)[j] = (*this)[j-1] + 1;
                        }
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
            }
        }


        const LoopArray imin;
        const LoopArray imax;
    private:
        Y_DISABLE_COPY_AND_ASSIGN(NestedLoop);
        void init() noexcept { for(size_t i=size();i>0;--i) (*this)[i] = imin[i]; }

    };
}
#endif

namespace {

    static inline
    size_t test3d(const size_t nmax)
    {
        size_t            n3d = 0;
        NestedLoop       loop(3,nmax);
        CxxArray<size_t> v(3,0);
        bool             res = true;
        for(size_t i=1;i<=nmax;++i)
        {
            for(size_t j=i+1;j<=nmax;++j)
            {
                for(size_t k=j+1;k<=nmax;++k)
                {
                    v[1] = i; v[2] = j; v[3] = k;
                    std::cerr << v << "/" << v << std::endl;
                    Y_ASSERT(loop==v);
                    ++n3d;
                    res = loop.next();
                }
            }
        }
        Y_ASSERT(!res);
        std::cerr << "n(3," << nmax << ")=" << n3d << std::endl;
        return n3d;
    }

    static inline
    size_t test4d(const size_t nmax)
    {
        size_t            n4d = 0;
        NestedLoop        loop(4,nmax);
        CxxArray<size_t>  v(4,0);
        bool              res = true;
        for(size_t i=1;i<=nmax;++i)
        {
            for(size_t j=i+1;j<=nmax;++j)
            {
                for(size_t k=j+1;k<=nmax;++k)
                {
                    for(size_t l=k+1;l<=nmax;++l)
                    {
                        v[1] = i; v[2] = j; v[3] = k; v[4] = l;
                        std::cerr << v << "/" << v << std::endl;
                        Y_ASSERT(loop==v);
                        ++n4d;
                        res = loop.next();
                    }
                }
            }
        }
        Y_ASSERT(!res);
        std::cerr << "n(4," << nmax << ")=" << n4d << std::endl;
        return n4d;
    }


}

#include "y/text/ascii/convert.hpp"
#include "y/stream/libc/output.hpp"

Y_UTEST(counting_nested)
{

    size_t nmax = 4;
    if(argc>1) nmax = ASCII::Convert::To<size_t>(argv[1],"nmax");
    {
        OutputFile fp("n3d.dat");
        for(size_t n=3;n<=nmax;++n)
        {
            const size_t n3d = test3d(n);
            fp("%u %u\n", unsigned(n), unsigned(n3d)).flush();
        }
    }

    {
        OutputFile fp("n4d.dat");
        for(size_t n=4;n<=nmax+1;++n)
        {
            const size_t n4d = test4d(n);
            fp("%u %u\n", unsigned(n), unsigned(n4d)).flush();
        }
    }


    for(size_t d=1;d<=10;++d)
    {
        std::cerr << "d=" << d << std::endl;
        for(size_t n=d;n<=20;++n)
        {
            std::cerr << "\tn=" << n << " => " << NestedLoop::CardinalityFor(d,n) << std::endl;
        }
    }




}
Y_UDONE()
