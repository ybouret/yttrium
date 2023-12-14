//! \file

#ifndef Y_MKL_Tao_Transmogrify_Included
#define Y_MKL_Tao_Transmogrify_Included 1

#include "y/type/conversion.hpp"

namespace Yttrium
{
    namespace MKL
    {
        //! \namespace Tao
        namespace Tao
        {
            //__________________________________________________________________
            //
            //! Relationship between target and source
            //__________________________________________________________________
            enum Relationship
            {
                IsSubClassOf, //!<   Y_Is_SuperSubClass(SOURCE,TARGET)
                MustCastFrom  //!< ! Y_Is_SuperSubClass(SOURCE,TARGET)
            };

            //! \namespace Cog
            namespace Cog
            {
                //______________________________________________________________
                //
                //
                //! base class for info
                //
                //______________________________________________________________
                class TransmogrifyInfo
                {
                public:
                    static const char *ToText(const Relationship) noexcept; //!< human readable

                    inline explicit TransmogrifyInfo() noexcept {} //!< do nothing
                    inline virtual ~TransmogrifyInfo() noexcept {} //!< do nothing
                private:
                    Y_DISABLE_COPY_AND_ASSIGN(TransmogrifyInfo);
                };
            }

            //__________________________________________________________________
            //
            //
            //! specific info
            //
            //__________________________________________________________________
            template <Relationship R> class TransmogrifyInfo : public Cog::TransmogrifyInfo
            {
            public:
                static const Relationship Status = R;                           //!< alias
                static inline const char *Text() noexcept { return ToText(R); } //!< human readable

                inline explicit TransmogrifyInfo() noexcept {} //!< do nothing
                inline virtual ~TransmogrifyInfo() noexcept {} //!< do nothing
            private:
                Y_DISABLE_COPY_AND_ASSIGN(TransmogrifyInfo);
            };



            namespace Cog
            {
                template <typename TARGET,Relationship,typename SOURCE> class To;

                //______________________________________________________________
                //
                //
                //! TARGET is sub-class of SOURCE => untouched
                //
                //______________________________________________________________
                template <typename TARGET,typename SOURCE>
                class To<TARGET,IsSubClassOf,SOURCE> : public Tao::TransmogrifyInfo<IsSubClassOf>
                {
                public:
                    typedef TARGET & ReturnType;
                    static inline ReturnType  Get(const SOURCE &source) noexcept { return source; }

                    inline explicit To() noexcept {} //!< do nothing
                    inline virtual ~To() noexcept {} //!< do nothing

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(To);
                };

                //______________________________________________________________
                //
                //
                //! TARGET does not derive from SOURCE => cast
                //
                //______________________________________________________________
                template <typename TARGET, typename SOURCE>
                class To<TARGET,MustCastFrom,SOURCE> : public Tao::TransmogrifyInfo<MustCastFrom>
                {
                public:
                    typedef TARGET ReturnType;
                    static inline  ReturnType Get(const SOURCE &source) noexcept { return TARGET(source); }

                    inline explicit To() noexcept {} //!< do nothing
                    inline virtual ~To() noexcept {} //!< do nothing

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(To);
                };
            }


            //__________________________________________________________________
            //
            //
            //! Relation TARGET/SOURCE
            //
            //__________________________________________________________________
            template <typename TARGET, typename SOURCE>
            struct Relation
            {
                //! alias
                static const Relationship Status = Y_Is_SuperSubClass(SOURCE,TARGET) ? IsSubClassOf : MustCastFrom;
            };


            //__________________________________________________________________
            //
            //
            //! Low-Level info and conversions
            //
            //__________________________________________________________________
            template <typename TARGET,typename SOURCE>
            class To : public Cog::To<TARGET, Relation<TARGET,SOURCE>::Status, SOURCE>
            {
            public:
                inline explicit To() noexcept {} //!< do nothing
                inline virtual ~To() noexcept {} //!< do nothing

            private:
                Y_DISABLE_COPY_AND_ASSIGN(To);
            };


            //__________________________________________________________________
            //
            //
            //! High-Level operations
            //
            //__________________________________________________________________
            template <typename RES, typename LHS, typename RHS>
            struct Transmogrify
            {

                //! product
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
