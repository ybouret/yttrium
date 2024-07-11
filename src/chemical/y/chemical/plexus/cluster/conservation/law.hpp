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
                

                void viz(OutputStream &fp, const String &color) const;//!< assume species are in fp

                bool linkedTo(const Species &) const noexcept; //!< this->hired(species)
                bool linkedTo(const Law     &) const noexcept; //!< this hired one of other's species

                xreal_t excess(const XReadable &C, const Level level, XAdd &xadd) const;

                bool    broken(xreal_t &         score,
                               XWritable &       Cout,
                               const Level       Lout,
                               const XReadable & Cinp,
                               const Level       Linp,
                               XAdd             &xadd) const;

#if 0
                xreal_t inject(const xreal_t factor, XWritable &deltaC, const Level level, XAdd &xadd) const;
#endif
                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                const  xreal_t         xden; //!< |nu|^2
                const Matrix<xreal_t > proj; //!< in group
                const SList            keep; //!< species in group, not in law
                Law *                  next; //!< for list
                Law *                  prev; //!< for list

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Law);
                
            };
        }
    }
}


#endif

