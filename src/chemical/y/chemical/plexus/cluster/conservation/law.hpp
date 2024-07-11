//! \file

#ifndef Y_Chemical_Conservation_Law_Included
#define Y_Chemical_Conservation_Law_Included 1

#include "y/chemical/reactive/actors.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Conservation
        {
            //__________________________________________________________________
            //
            //
            //
            //! conservation law as Actors
            //
            //
            //__________________________________________________________________
            class Law :  public  Actors
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef CxxListOf<Law> List; //!< alias

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________

                //! setup from species and some coefficients
                explicit Law(const SList              &species,
                             const Readable<unsigned> &coef);
                virtual ~Law() noexcept; //!< cleanup
                Y_OSTREAM_PROTO(Law);    //!< display

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________
                
                //! assume species are in fp
                void viz(OutputStream &fp, const String &color) const;

                bool linkedTo(const Species &) const noexcept;
                bool linkedTo(const Law     &) const noexcept;

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                Law    *next; //!< for list
                Law    *prev; //!< for list

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Law);
                
            };
        }
    }
}


#endif

