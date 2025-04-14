
//! \file


#ifndef Y_Chemical_Boundary_Included
#define Y_Chemical_Boundary_Included 1

#include "y/chemical/plexus/equalizing/restartable.hpp"
#include "y/chemical/plexus/equalizing/hauler.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Equalizing
        {
            class Cursor;
            
            //__________________________________________________________________
            //
            //
            //
            //! extent shared for (multiple) species
            //
            //
            //__________________________________________________________________
            class Boundary : public Hauler, public Restartable
            {
            public:
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit Boundary(const SBank &) noexcept; //!< setup empty
                virtual ~Boundary()              noexcept; //!< cleanup
                Boundary(const Boundary &);                //!< duplicate
                Boundary&operator=(const Boundary &);      //!< assign by copy/xch
                Boundary&operator=(const Cursor   &);      //!< duplicate and xch
                Y_OSTREAM_PROTO(Boundary);                 //!< display
                
                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

                //! return xi
                virtual xreal_t getXi() const noexcept;


                //! process species with its extent
                void operator()(const Species &sp, const xreal_t xx);
                
                //! free and xi=0
                virtual void restart() noexcept;
                
                //! no throw exchange
                void xch(Boundary &_) noexcept;
                
                //! add species
                void add(const Cursor&);
                

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                xreal_t xi; //!< current extent
                
            };
            
            
        }

    }

}


#endif
