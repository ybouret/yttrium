
//! \file

#ifndef Y_Chemical_Actors_Included
#define Y_Chemical_Actors_Included 1

#include "y/chemical/reactive/actor.hpp"
#include "y/type/proxy.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! List of Actors
        //
        //
        //______________________________________________________________________
        class Actors : public Entity, public Proxy<const Actor::List>
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Actors();           //!< setup with top-level index
            virtual ~Actors() noexcept;  //!< cleanup
            Y_OSTREAM_PROTO(Actors);     //!< display

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! add next actor
            void operator()(const unsigned nu, const Species &sp);

            //! compute mass action
            void massAction(XMul &xmul, const XReadable &C, const Level level) const;
           
            //! compute mass action of C0+nu*xi
            void massAction(XMul            &xmul,
                            const XReadable &C0,
                            const xreal_t    xi,
                            const Level      level) const;

            

            //! check each concentration is positive
            bool wouldRun(const XReadable &C0, const Level level) const;


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Actors);
            Actor::List actors;
            virtual ConstInterface & surrogate() const noexcept;
            void growName(const String &);

        };

    }

}

#endif

