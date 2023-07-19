
#include "y/ptr/auto.hpp"
#include "y/utest/run.hpp"
#include "y/object.hpp"

using namespace Yttrium;

namespace
{
    class Info
    {
    public:
        Info() noexcept : data(0) {}
        virtual ~Info() noexcept  {}

        int data;


    private:
        Y_DISABLE_COPY_AND_ASSIGN(Info);
    };

    class Dummy : public Object, public Info
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

        Info *       operator->() noexcept { return this; }
        const Info * operator->() const noexcept { return this; }

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
        std::cerr << pDum->count << std::endl;
        const AutoPtr<Dummy> &cst = pDum;
        std::cerr << cst->count << std::endl;
    }

    {
        AutoPtr<const Dummy> pDum = new Dummy();
        std::cerr << (*pDum).count << std::endl;
        pDum = new Dummy();
        std::cerr << (*pDum).count << std::endl;
    }

    {
        AutoPtr<Dummy,DrillDown> pDum = 0;
        pDum = new Dummy();
        std::cerr << pDum->data << std::endl;
    }

}
Y_UDONE()
