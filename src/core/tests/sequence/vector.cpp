
#include "y/sequence/vector.hpp"
#include "y/utest/run.hpp"
#include "y/random/shuffle.hpp"
#include "y/string.hpp"
#include "y/container/iterator/display-all.hpp"

using namespace Yttrium;

namespace
{
    class Dummy
    {
    public:
        static int Count;

        const int count;
        Dummy()  noexcept : count(++Count) { std::cerr << "+dummy[" << count << "]" << std::endl; }
        ~Dummy() noexcept { --Count; std::cerr << "-dummy[" << count << "]" << std::endl; }

        Dummy(const Dummy &dum) noexcept : count(++Count)
        {
            std::cerr << "+dummy[" << count << "] <== [" << dum.count << "]" << std::endl;
        }

        friend std::ostream & operator<<( std::ostream &os, const Dummy &dum)
        {
            os << dum.count;
            return os;
        }

        void show() const
        {
            std::cerr << "dum=" << *this << std::endl;
        }

    private:
        Y_DISABLE_ASSIGN(Dummy);
    };


    int Dummy:: Count = 0;

}

Y_UTEST(sequence_vector)
{
    Random::Rand ran;




    {
        Vector<float> v;
        for(size_t loop=0;loop<16;++loop)
        {
            for(size_t n=3+ran.leq(64);n>0;--n)
            {
                if(ran.choice())
                {
                    v.pushHead( ran.to<float>() );
                }
                else
                {
                    v.pushTail( ran.to<float>() );
                }
                std::cerr << v << " " << v.size() << " / " << v.capacity() << std::endl;
            }
            while(v.size()>0)
            {
                if(ran.choice()) v.popHead(); else v.popTail();
                std::cerr << v << std::endl;
            }
            {
                const Vector<float> tmp = v;
                Y_CHECK(tmp.size()==v.size());
            }
            v.release();
        }
    }

    {
        Vector<String> v;
        Vector<String> u;

        for(size_t loop=0;loop<16;++loop)
        {
            for(size_t n=3+ran.leq(64);n>0;--n)
            {
                const String s(1+ran.leq(4),ran);
                if(ran.choice())
                {
                    v.pushHead( s );
                }
                else
                {
                    v.pushTail( s );
                }
            }
            while(v.size()>0)
            {
                if(ran.choice()) v.popHead(); else v.popTail();
                std::cerr << v << std::endl;
            }
            {
                const Vector<String> tmp = v;
                Y_CHECK(v==tmp);
            }
            u.free();
            u = v;
            Y_CHECK(u==v);
            v.release();
        }
    }

    {
        Vector<Dummy> v;
        {
            const Dummy   d;
            for(size_t n=64;n>0;--n)
            {
                if(ran.choice())
                {
                    v.pushHead( d );
                }
                else
                {
                    v.pushTail( d );
                }
                std::cerr << v << std::endl;
            }
        }
        std::cerr << "done growth" << std::endl;
        {
            const Vector<Dummy> tmp(v);
            Y_CHECK(tmp.size()==v.size());
        }

        Iterating::DisplayAll::Of(v);

        v.forEach( & Dummy::show );

        v.release();
    }
    Y_CHECK(Dummy::Count==0);


}
Y_UDONE()

