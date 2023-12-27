
#include "y/mkl/filter/resago.hpp"
#include "y/type/nullify.hpp"
#include "y/mkl/algebra/lu.hpp"
#include "y/associative/little-endian-key.hpp"
#include "y/ptr/ark.hpp"
#include "y/associative/suffix/set.hpp"

namespace Yttrium
{
    namespace MKL
    {

       


        class ReSaGo::Factory::Code : public Object
        {
        public:
            inline explicit Code() : Object(), lu() {}
            inline virtual ~Code() noexcept {}

            LU<apq> lu;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);
        };

        ReSaGo:: Factory::  Factory() : code( new Code() )
        {
        }

        ReSaGo:: Factory:: ~Factory() noexcept
        { assert(0!=code); Nullify(code); }

    }

}
