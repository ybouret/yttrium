//! \file

#ifndef Y_MKL_Tao_Transmogrify_Included
#define Y_MKL_Tao_Transmogrify_Included 1

#include "y/type/conversion.hpp"

namespace Yttrium
{
    namespace MKL
    {
        namespace Tao
        {


            enum Relationship
            {
                IsSubClassOf,
                MustCastFrom
            };

            namespace Cog
            {
                class TransmogrifyInfo
                {
                public:
                    static const char *ToText(const Relationship) noexcept;
                };
            }

            template <Relationship R> class TransmogrifyInfo : public Cog::TransmogrifyInfo
            {
            public:
                static const Relationship Status = R;
                static inline const char *Text() noexcept { return ToText(R); }
            };



            namespace Cog
            {
                template <typename TARGET,Relationship,typename SOURCE> class To;


                //! TARGET is sub-class of SOURCE => untouched
                template <typename TARGET,typename SOURCE>
                class To<TARGET,IsSubClassOf,SOURCE> : public Tao::TransmogrifyInfo<IsSubClassOf>
                {
                public:
                    typedef TARGET & ReturnType;
                    static inline ReturnType  Get(const SOURCE &source) noexcept { return source; }
                };

                //! TARGET does not derive from SOURCE => cast
                template <typename TARGET, typename SOURCE>
                class To<TARGET,MustCastFrom,SOURCE> : public Tao::TransmogrifyInfo<MustCastFrom>
                {
                public:
                    typedef TARGET ReturnType;
                    static inline  ReturnType Get(const SOURCE &source) noexcept { return TARGET(source); }
                };
            }


            template <typename TARGET, typename SOURCE>
            struct Relation
            {
                static const Relationship Status = Y_Is_SuperSubClass(SOURCE,TARGET) ? IsSubClassOf : MustCastFrom;
            };

            template <typename TARGET,typename SOURCE>
            class To : public Cog::To<TARGET, Relation<TARGET,SOURCE>::Status, SOURCE>
            {

            };

            template <typename RES, typename LHS, typename RHS>
            struct Transmogrify
            {

                static inline RES Product(const LHS &lhs,
                                          const RHS &rhs)
                {
                    const LHS product = lhs * To<LHS,RHS>::Get(rhs);
                    const RES result  = To<RES,LHS>::Get(product);
                    return    result;
                }


            };





        }
    }

}

#endif
