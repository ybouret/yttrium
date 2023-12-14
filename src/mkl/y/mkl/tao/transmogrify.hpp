//! \file

#ifndef Y_MKL_Tao_Transmogrify_Included
#define Y_MKL_Tao_Transmogrify_Included 1

#include "y/type/conversion.hpp"
#include "y/type/args.hpp"

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
                    static const char *ToText(const Relationship) noexcept;   //!< human readable
                    inline explicit    TransmogrifyInfo()         noexcept {} //!< do nothing
                    inline virtual    ~TransmogrifyInfo()         noexcept {} //!< do nothing
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
                //  prototype
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
                    //__________________________________________________________
                    //
                    //
                    // Definitions
                    //
                    //__________________________________________________________
                    typedef typename TypeTraits<TARGET>::MutableType MutableTarget; //!< alias
                    typedef const MutableTarget                      ConstTarget;   //!< alias
                    typedef typename TypeTraits<SOURCE>::MutableType MutableSource; //!< alias
                    typedef const MutableSource                      ConstSource;   //!< aluas
                    typedef ConstTarget &                            ReturnType;    //!< return type

                    //__________________________________________________________
                    //
                    //
                    // Functions
                    //
                    //__________________________________________________________
                    static inline ReturnType  Get(ConstSource &source) noexcept { return source; }  //!< source to target

                    //__________________________________________________________
                    //
                    //
                    // C++
                    //
                    //__________________________________________________________
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
                    //__________________________________________________________
                    //
                    //
                    // Definitions
                    //
                    //__________________________________________________________
                    typedef typename TypeTraits<TARGET>::MutableType MutableTarget; //!< alias
                    typedef const MutableTarget                      ConstTarget;   //!< alias
                    typedef typename TypeTraits<SOURCE>::MutableType MutableSource; //!< alias
                    typedef const MutableSource                      ConstSource;   //!< aluas
                    typedef ConstTarget                              ReturnType;    //!< return type

                    //__________________________________________________________
                    //
                    //
                    // Functions
                    //
                    //__________________________________________________________
                    static inline  ReturnType Get(ConstSource &source) noexcept { return TARGET(source); } //!< source to target

                    //__________________________________________________________
                    //
                    //
                    // C++
                    //
                    //__________________________________________________________
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
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef typename TypeTraits<TARGET>::MutableType MutableTarget; //!< alias
                typedef typename TypeTraits<SOURCE>::MutableType MutableSource; //!< alias

                //! deduce status
                static const Relationship Status = Y_Is_SuperSubClass(MutableSource,MutableTarget) ? IsSubClassOf : MustCastFrom;
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
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                inline explicit To() noexcept {} //!< do nothing
                inline virtual ~To() noexcept {} //!< do nothing

            private:
                Y_DISABLE_COPY_AND_ASSIGN(To);
            };


            //__________________________________________________________________
            //
            //
            //! Cascading transforms to get RES from LHS and RHS
            //
            //__________________________________________________________________
            template <typename RES>
            struct Transmogrify
            {
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef typename TypeTraits<RES>::MutableType MutableRES; //!< alias
                typedef const    MutableRES                   ConstRES;   //!< aluas

                //______________________________________________________________
                //
                //
                // Functions
                //
                //______________________________________________________________

                //! successive calls to compute product
                template <typename LHS, typename RHS> static inline
                ConstRES Product(LHS &lhs, RHS &rhs)
                {
                    typedef typename TypeTraits<LHS>::MutableType MutableLHS;
                    typedef const MutableLHS                      ConstLHS;
                    ConstLHS product = lhs * To<LHS,RHS>::Get(rhs);
                    ConstRES result  = To<RES,LHS>::Get(product);
                    return   result;
                }

            };




        }
    }

}

#endif
