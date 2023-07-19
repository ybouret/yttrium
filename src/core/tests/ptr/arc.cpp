
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


    private:
        Y_DISABLE_COPY_AND_ASSIGN(Dummy);
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

}
Y_UDONE()
