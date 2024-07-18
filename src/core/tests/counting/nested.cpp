#include "y/container/cxx/array.hpp"
#include "y/utest/run.hpp"


using namespace Yttrium;

namespace Yttrium
{
    typedef CxxArray<size_t> LoopArray;

    class NestedLoop : public LoopArray
    {
    public:

        explicit NestedLoop(const size_t dims, 
                            const size_t nmax) :
        LoopArray(dims,0),
        maxi(nmax)
        {
            init();
        }

        void reset() noexcept { init(); }

        virtual ~NestedLoop() noexcept {}

        bool next()
        {
            const size_t      n = size();
            Writable<size_t> &u = *this;

            for(size_t i=n;i>0;--i)
            {
                size_t &j = u[i];
                if(j<maxi)
                {
                    // update local index
                    ++j;
                    return true;
                }
                else
                {
                    return false;
                }
            }

            return false;
        }

        const size_t maxi;


    private:
        Y_DISABLE_COPY_AND_ASSIGN(NestedLoop);
        void init() noexcept { for(size_t i=size();i>0;--i) (*this)[i] = i; }
    };
}

Y_UTEST(counting_nested)
{

    NestedLoop loop(3,4);

    std::cerr << loop << std::endl;
    while(loop.next())
    {
        std::cerr << loop << std::endl;
    }

}
Y_UDONE()
