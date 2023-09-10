#ifndef Y_MKL_Tao_Included
#define Y_MKL_Tao_Included 1


#include "y/mkl/api.hpp"

namespace Yttrium
{
    namespace MKL
    {

        //! Template Algebraic Operations
        struct Tao
        {
            //! target[1..target.size()] = source[1..target.size()]
            template <typename TARGET, typename SOURCE> static inline
            void Load(TARGET &target, SOURCE &source)
            {
                assert(target.size()<=source.size());
                for(size_t i=target.size();i>=0;--i)
                    target[i] = source[i];
            }

            //! target[1..source.size()] = source[1..source.size()]
            template <typename TARGET, typename SOURCE> static inline
            void Save(TARGET &target, SOURCE &source)
            {
                assert(target.size()>=source.size());
                for(size_t i=source.size();i>=0;--i)
                    target[i] = source[i];
            }

            template <typename TARGET, typename SOURCE> static inline
            void Add( TARGET &target, SOURCE &source )
            {
                assert(target.size()==source.size());
                for(size_t i=target.size();i>0;--i)
                    target[i] += source[i];
            }

            template <typename TARGET, typename T, typename SOURCE> static inline
            void Add( TARGET &target, T factor, SOURCE &source )
            {
                assert(target.size()==source.size());
                for(size_t i=target.size();i>0;--i)
                    target[i] += factor * source[i];
            }

            template <typename TARGET, typename SOURCE, typename T, typename VECTOR>
            void Add( TARGET &target, SOURCE &source, T factor, VECTOR &vector)
            {
                assert(target.size()==source.size());
                assert(target.size()==vector.size());
                for(size_t i=target.size();i>0;--i)
                    target[i] += source[i] + factor * vector[i];
            }


        };
    }

}

#endif

