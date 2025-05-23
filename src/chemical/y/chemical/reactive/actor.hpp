
//! \file


#ifndef Y_Chemical_Actor_Included
#define Y_Chemical_Actor_Included 1


#include "y/chemical/species.hpp"
#include "y/chemical/type/defs.hpp"


namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! Single Actor
        //
        //
        //______________________________________________________________________
        class Actor : public Entity
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________

            //! for components or concentration
            enum Involvement
            {
                AsComponentOnly, //!< for components
                AsConcentration  //!< for conservations,...
            };

            static const char * const CallSign; //!< "Actor"
            typedef CxxListOf<Actor>  List;     //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup
            /**
             \param _nu coefficient
             \param _sp persistent species
             \param _in involvement
             */
            explicit Actor(const unsigned    _nu,
                           const Species &   _sp,
                           const Involvement _in);
            Actor(const Actor &) noexcept; //!< copy with shared entity name
            virtual ~Actor()     noexcept; //!< cleanup


            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void    activity(XMul &X, const XReadable &C, const Level L)                   const; //!< X.insert(C,nu)
            void    activityError(XMul &X, const XReadable &C, const Level L)              const; //!< X << nu^2 * C^(nu-1)
            void    activity(XMul &X, const XReadable &C, const Level L, const xreal_t xi) const; //!< clipped
            xreal_t limiting(const XReadable &C, const Level L)             const noexcept;       //!< cc/xn
            void    safeMove(XWritable &C, const Level L, const xreal_t xi) const noexcept;       //!< update valid  C
            void    boldMove(XWritable &C, const Level L, const xreal_t xi) const noexcept;       //!< update invalid C
            void    drvsActivity(XMul &X, const XReadable &C, const Level L) const;               //!< X << nu * C^(nu-1)

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const unsigned    nu;   //!< coefficient
            const xreal_t     xn;   //!< nu as xreal_t
            const Species &   sp;   //!< persistent species
            const unsigned    nm1;  //!< nu-1
            const xreal_t     xn2;  //!< xn^2
            Actor *           next; //!< for list
            Actor *           prev; //!< for list

        private:
            Y_DISABLE_ASSIGN(Actor);
        };
    }

}

#endif
