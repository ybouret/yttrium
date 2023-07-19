
#include "y/ptr/auto.hpp"
#include "y/utest/run.hpp"
#include "y/object.hpp"

using namespace Yttrium;

namespace
{
    class Info
    {
    public:
    private:
        Y_DISABLE_COPY_AND_ASSIGN(Info);
    };

    class Dummy : public Object
    {
    public:
        static int Count;

        explicit Dummy() noexcept : Object(), count(++Count)
        {
            std::cerr << "+[dummy@" << count << "/" << Count << "]" << std::endl;
        }
        virtual ~Dummy() noexcept {
            std::cerr << "-[dummy@" << count << "/" << Count << "]" << std::endl;
            --Count;
        }

        const int count;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Dummy);
    };

    int Dummy::Count = 0;

}

Y_UTEST(ptr_auto)
{

    {
        AutoPtr<Dummy> pDum = 0;
        Y_CHECK(pDum.isEmpty());
        Y_CHECK(!pDum.isValid());
    }

    {
        AutoPtr<Dummy> pDum = new Dummy();
        std::cerr << (*pDum).count << std::endl;
        pDum = new Dummy();
        std::cerr << (*pDum).count << std::endl;

    }

}
Y_UDONE()
