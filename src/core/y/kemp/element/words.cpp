
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

            const size_t np = source.positive;
            const size_t nl = np >> 1;
            const size_t nu = np-nl;

            lower = new Element( nl * sizeof(T), AsCapacity);
            upper = new Element( nu * sizeof(T), AsCapacity);

            Assembly<T> &L = lower->get<T>();
            Assembly<T> &U = upper->get<T>();

            for(size_t i=0;i<nl;++i)
                L.item[i] = source.item[i];
            L.positive  = nl;
            lower->bits = L.updateBits();
            lower->revise();

            for(size_t i=0;i<nu;++i)
                U.item[i] = source.item[nl+i];
            U.positive = nu;
            upper->bits = U.updateBits();
            upper->revise();
            
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