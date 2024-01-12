
#include "y/type/list.hpp"
#include "y/type/traits.hpp"
#include "y/utest/run.hpp"
#include "y/string.hpp"


using namespace Yttrium;

namespace Yttrium
{

#define Y_BINDER_DECL(I) \
typedef typename TL::SafeTypeAt<TLIST,I-1,EmptyType>::Result Type##I;\
typedef typename TypeTraits<Type##I>::ParamType              Param##I;\
Type##I                                                      arg##I

    template <typename TLIST>
    class Binder
    {
    public:
        Y_BINDER_DECL(1);
        Y_BINDER_DECL(2);
        Y_BINDER_DECL(3);
        Y_BINDER_DECL(4);


        inline explicit Binder() : arg1(), arg2()
        {}

        inline explicit Binder(Param1 p1) : arg1(p1), arg2() {}

        inline virtual ~Binder() noexcept {}

        inline friend std::ostream & operator<<(std::ostream &os, const Binder &self)
        {
            os << '[';

            os << ' ' <<        self.arg1;
            os << ',' << ' ' << self.arg2;
            os << ',' << ' ' << self.arg3;
            os << ',' << ' ' << self.arg4;

            os << ' ' << ']';
            return os;
        }



    private:
        Y_DISABLE_COPY_AND_ASSIGN(Binder);
    };


}

Y_UTEST(type_binder)
{
    Binder<TL1(const int)> ib(2); std::cerr << ib.arg1 << std::endl;
    Binder<NullType>       nb;

    std::cerr << ib << std::endl;
    Y_SIZEOF(ib);
    std::cerr << nb << std::endl;
    Y_SIZEOF(nb);

}
Y_UDONE()

