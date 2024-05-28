#include "y/apk/natural.hpp"
#include "y/apk/component.hpp"
#include "y/type/nullify.hpp"
#include "y/apk/natural/latch.hpp"
#include "y/ptr/auto.hpp"

namespace Yttrium
{
    namespace APK
    {

        class Natural::Code : public Object
        {
        public:
            inline explicit Code() :
            Object(),
            self( new Component() ),
            bolt()
            {
            }

            inline explicit Code(const uint64_t u) :
            Object(),
            self( new Component(FromNum64,u) ),
            bolt()
            {
            }

            inline explicit Code(const Code &other) :
            Object(),
            self( other.duplicate() ),
            bolt()
            {
            }

            inline virtual ~Code() noexcept {}

            inline Component * duplicate() const { return new Component(*self); }

            AutoPtr<Component> self;
            Latch::Handle      bolt;


        private:
            Y_DISABLE_ASSIGN(Code);

        };

        Natural:: ~Natural() noexcept
        {
            assert(0!=code);
            Nullify(code);
        }

        Natural:: Natural() :
        Number(),
        code( new Code() )
        {
        }

        Natural:: Natural(const Natural &other) :
        Number(),
        code( new Code(*other.code) )
        {
        }

        Natural & Natural:: operator=(const Natural &other)
        {
            AutoPtr<Component> here = other.code->duplicate();
            code->self.xch(here);
            return *this;
        }

        Natural:: Natural(const uint64_t qw) :
        code( new Code(qw) )
        {

        }


    }

}


