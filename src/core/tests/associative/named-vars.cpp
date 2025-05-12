
#include "y/ptr/ark.hpp"
#include "y/string.hpp"
#include "y/utest/run.hpp"
#include "y/type/proxy.hpp"
#include "y/memory/out-of-reach.hpp"
#include "y/associative/suffix/set.hpp"

namespace Yttrium
{

    namespace Named
    {

        template <typename T>
        class Variable : public Object, public Counted, public Memory::ReadOnlyBuffer
        {
        public:
            Y_ARGS_EXPOSE(T,Type);

            template <typename NAME>
            inline explicit Variable(const NAME &n, ConstType u) : name(n), uuid(u) {}
            inline virtual ~Variable() noexcept {}

            inline friend std::ostream & operator<<(std::ostream &os, const Variable &v)
            {
                os << v.name << '=' << v.uuid;
                return os;
            }

            inline virtual const void *ro_addr() const noexcept { return Memory::OutOfReach::Addr(&uuid); }
            inline virtual size_t      measure() const noexcept { return sizeof(T); }

            const String name;
            ConstType    uuid;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Variable);
        };


        template <typename VARIABLE>
        class ByLabel : public ArcPtr<const VARIABLE>
        {
        public:
            typedef ArcPtr<const VARIABLE> Pointer;
            typedef String                 KeyType;

            inline explicit ByLabel(const VARIABLE * const _) noexcept : Pointer(_) {}
            inline virtual ~ByLabel() noexcept {}
            inline ByLabel(const ByLabel &_) noexcept : Pointer(_) {}

            inline const KeyType & key() const noexcept { return (**this).name; }

        private:
            Y_DISABLE_ASSIGN(ByLabel);
        };

        template <typename VARIABLE>
        class ByValue : public ArcPtr<const VARIABLE>
        {
        public:
            typedef ArcPtr<const VARIABLE> Pointer;
            typedef Memory::ReadOnlyBuffer KeyType;

            inline explicit ByValue(const ByLabel<VARIABLE> &_) noexcept : Pointer(_) {}
            inline virtual ~ByValue() noexcept {}
            inline ByValue(const ByValue &_) noexcept : Pointer(_) {}

            inline const KeyType & key() const noexcept { return (**this); }

        private:
            Y_DISABLE_ASSIGN(ByValue);
        };

    }

}

using namespace Yttrium;


Y_UTEST(associative_named_vars)
{

    typedef Named::Variable<int> iVar;

    Named::ByLabel<iVar> blv( new iVar("one",1) );
    Named::ByValue<iVar> bvv( blv );

    std::cerr << blv << " / " << bvv << std::endl;
    Y_CHECK( & *blv == & *bvv );


}
Y_UDONE()
