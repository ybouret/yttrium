//! \file


#ifndef Y_Chemical_Actors_Included
#define Y_Chemical_Actors_Included 1


#include "y/chemical/reactive/actor.hpp"
#include "y/chemical/type/latchable.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! List of actors
        //
        //
        //______________________________________________________________________
        class Actors : public Entity, public Proxy<const Actor::List>, public Latchable
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const CallSign; //!< "Actors"

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Actors(const Actor::Involvement); //!< setup
            virtual ~Actors() noexcept;                //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            const Actor & operator()(const unsigned nu, const Species &sp); //!< create new actor
            bool          has(const Species &) const noexcept;              //!< check presence
            static void   Exchange(Actors &lhs, Actors &rhs) noexcept;      //!< same involments

            void    activity(XMul &X, const XReadable &C, const Level L)                   const; //!< gather activities in X
            void    activity(XMul &X, const XReadable &C, const Level L, const xreal_t xi) const; //!< gather activities in X
            bool    critical(const XReadable &C, const Level L)                   const noexcept; //!< at least one zero concentration
            xreal_t limiting(const XReadable &C, const Level L)                   const noexcept; //!< fast limiting extent
            void    moveSafely(XWritable &C, const Level L, const xreal_t xi)     const noexcept; //!< update

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const Actor::Involvement in; //!< involvement

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Actors);
            Y_PROXY_DECL();
            Actor::List my; //!< my list
        public:
            const unsigned sum; //!< sum(nu);
            const real_t   kxp; //!< 1.0 / sum

        };
    }

}

#endif
