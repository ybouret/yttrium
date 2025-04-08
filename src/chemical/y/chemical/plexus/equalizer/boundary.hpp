
//! \file


#ifndef Y_Chemical_Boundary_Included
#define Y_Chemical_Boundary_Included 1

#include "y/chemical/plexus/equalizer/restartable.hpp"
#include "y/chemical/type/defs.hpp"
#include "y/chemical/species.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Equalizer
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
            class Boundary : public SRepo, public Restartable
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
