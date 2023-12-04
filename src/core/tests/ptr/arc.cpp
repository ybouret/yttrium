
#include "y/ptr/arc.hpp"
#include "y/utest/run.hpp"
#include "y/object.hpp"
#include "y/counted.hpp"

using namespace Yttrium;

namespace
{
    class Dummy : public Object, public Counted
    {
    public:
        explicit Dummy() noexcept : Object(), Counted() {}
        virtual ~Dummy() noexcept {}

        virtual const char *name() const noexcept { return "Dummy"; }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Dummy);
    };

    class Derived : public Dummy
    {
    public:
        explicit Derived() noexcept : Dummy() {}
        virtual ~Derived() noexcept {}

        virtual const char *name() const noexcept { return "Derived"; }


    private:
        Y_DISABLE_COPY_AND_ASSIGN(Derived);
    };

}
Y_UTEST(ptr_arc)
{
    {
        ArcPtr<Dummy> p = new Dummy();
        {
            const ArcPtr<Dummy> q(p);
            p = q;
        }
    }

    {
        const ArcPtr<Derived> derived = new Derived();
        const ArcPtr<Dummy>   dummy(CopyOf,derived);
        std::cerr << "name: " << dummy->name() << std::endl;
    }

}
Y_UDONE()
