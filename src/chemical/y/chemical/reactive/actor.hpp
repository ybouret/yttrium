//! \file

#ifndef Y_Chemical_Actor_Included
#define Y_Chemical_Actor_Included 1

#include "y/chemical/species.hpp"
#include "y/data/list/cxx.hpp"
#include "y/chemical/types.hpp"
#include "y/associative/address-book.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //
        //! Actor : nu>0 + species
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
            typedef CxxListOf<Actor> List; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup
            explicit Actor(const unsigned n,
                           const Species &s);
            virtual ~Actor() noexcept;         //!< cleanup
            Y_OSTREAM_PROTO(Actor);            //!< display

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            const Actor & operator*()  const noexcept; //!< *this for list
            void          record(AddressBook &) const; //!< record species
            int           charge()     const noexcept; //!< algebraic charge

            //! compute mass action
            void massAction(XMul            &xmul,
                            const XReadable &C,
                            const Level      level) const;


            //! compute mass action of C0 + nu*xi
            void massAction(XMul            &xmul,
                            const XReadable &C0,
                            const xreal_t    xi,
                            const Level      level) const;


            //! compute derivative of mass action
            void drvsMassAction(XMul            &xmul,
                                const XReadable &C,
                                const Level      level) const;
            


            //! C/nu, assuming C>=0
            xreal_t maxExtent(const XReadable &C, const Level level) const;

            //! C += nu*xi >= 0
            void    moveSafe(XWritable &C, const Level L, const xreal_t xi) const;

            //! Cout = (Cinp + xi * nu) >= 0
            void    addSafe(XWritable       &Cout,
                            const Level      Lout,
                            const XReadable &Cinp,
                            const Level     &Linp,
                            const xreal_t    xi) const;


            //! assuming species and peer are already in fp
            void    viz(OutputStream &             fp,
                        const Entity &             peer,
                        const String &             color,
                        const Iterating::Direction dir) const;

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const unsigned  nu;       //!< coefficient
            const xreal_t   xn;       //!< nu as xreal_t
            const Species  &sp;       //!< species
            const unsigned  n1;       //!< nu-1
            Actor *         next;     //!< for list
            Actor *         prev;     //!< for list

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Actor);
            void initialize();

        };

    }
}

#endif

