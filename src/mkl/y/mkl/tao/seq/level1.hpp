#ifndef Y_MKL_SeqTao1_Included
#define Y_MKL_SeqTao1_Included 1


#include "y/mkl/api.hpp"
#include "y/mkl/tao/multifold.hpp"

namespace Yttrium
{
    namespace MKL
    {

        namespace Tao
        {
            //__________________________________________________________________
            //
            //! target[1..target.size()] = source[1..target.size()]
            //__________________________________________________________________
            template <typename TARGET, typename SOURCE>   inline
            void Load(TARGET &target, SOURCE &source)
            {
                assert(target.size()<=source.size());
                for(size_t i=target.size();i>0;--i)
                    target[i] = source[i];
            }

            //__________________________________________________________________
            //
            //! target[1..source.size()] = source[1..source.size()]
            //__________________________________________________________________
            template <typename TARGET, typename SOURCE>   inline
            void Save(TARGET &target, SOURCE &source)
            {
                assert(target.size()>=source.size());
                for(size_t i=source.size();i>0;--i)
                    target[i] = source[i];
            }

        }


        namespace Tao
        {
            //__________________________________________________________________
            //
            //! target += source
            //__________________________________________________________________
            template <typename TARGET, typename SOURCE>   inline
            void Add( TARGET &target, SOURCE &source )
            {
                assert(target.size()==source.size());
                for(size_t i=target.size();i>0;--i)
                    target[i] += source[i];
            }


            //__________________________________________________________________
            //
            //! target += factor * source
            //__________________________________________________________________
            template <typename TARGET, typename T, typename SOURCE>   inline
            void Add( TARGET &target, T factor, SOURCE &source )
            {
                assert(target.size()==source.size());
                for(size_t i=target.size();i>0;--i)
                    target[i] += factor * source[i];
            }

            //__________________________________________________________________
            //
            //! target = source + factor * vector
            //__________________________________________________________________
            template <typename TARGET, typename SOURCE, typename T, typename VECTOR>   inline
            void Add( TARGET &target, SOURCE &source, T factor, VECTOR &vector)
            {
                assert(target.size()==source.size());
                assert(target.size()==vector.size());
                for(size_t i=target.size();i>0;--i)
                    target[i] += source[i] + factor * vector[i];
            }

        }

        namespace Tao
        {
#if 0
            //__________________________________________________________________
            //
            //! |source|^2
            //__________________________________________________________________
            template <typename SOURCE>   inline
            typename ScalarFor< typename SOURCE::Type >::Type Mod2(SOURCE &source)
            {
                typename ScalarFor< typename SOURCE::Type >::Type res(0);
                for(size_t i=source.size();i>0;--i)
                    res += MKL::Mod2<typename SOURCE::Type>::Of(source[i]);
                return res;
            }
#endif

            //__________________________________________________________________
            //
            //! |source|^2
            //__________________________________________________________________
            template <typename SOURCE>   inline
            typename ScalarFor< typename SOURCE::Type >::Type
            Mod2(SOURCE                                                         &source,
                 Multifold< typename ScalarFor< typename SOURCE::Type >::Type > &xlist)
            {
                typedef typename SOURCE::Type           Type;
                typedef typename ScalarFor<Type >::Type ScalarType;
                typedef  DynamicAdd<ScalarType>         XNode;
                typedef  typename XNode::XAdd           XAdd;
                const size_t  size =  source.size;
                XAdd         &xadd = *xlist.make(source.size);
                for(size_t i=size;i>0;--i)
                {
                    xadd << MKL::Mod2<Type>::Of(source[i]);
                }
                return xadd.sum();
            }


            //__________________________________________________________________
            //
            //! |primary-replica|^2
            //__________________________________________________________________
            template <typename PRIMARY, typename REPLICA>   inline
            typename ScalarFor<typename PRIMARY::Type>::Type Mod2(PRIMARY &primary, REPLICA &replica)
            {
                assert(primary.size()==replica.size());
                typename ScalarFor<typename PRIMARY::Type>::Type res(0);
                for(size_t i=primary.size();i>0;--i)
                {
                    typename PRIMARY::Type delta = primary[i] - replica[i];
                    res += MKL::Mod2<typename PRIMARY::Type>::Of(delta);
                }
                return res;
            }

        };
    }

}

#endif

