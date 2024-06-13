
#include "y/kemp/element.hpp"

namespace Yttrium
{
    namespace Kemp
    {

        template <typename T> static inline
        void Split(AutoPtr<Element>  &lower,
                   AutoPtr<Element>  &upper,
                   const Assembly<T> &source)
        {

            
        }

        void Element:: split(AutoPtr<Element> &lower,
                             AutoPtr<Element> &upper) const
        {
            switch( state )
            {
                case AsBytes: Split(lower,upper,bytes); break;
                case AsNum16: Split(lower,upper,num16); break;
                case AsNum32: Split(lower,upper,num32); break;
                case AsNum64: Split(lower,upper,num64); break;
            }
        }

    }

}
