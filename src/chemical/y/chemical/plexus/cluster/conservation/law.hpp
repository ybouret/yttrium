//! \file

#ifndef Y_Chemical_Conservation_Law_Included
#define Y_Chemical_Conservation_Law_Included 1

#include "y/chemical/reactive/equilibrium.hpp"

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

                bool linkedTo(const Species &)    const noexcept; //!< this->hired(species)
                bool linkedTo(const Law     &)    const noexcept; //!< this hired one of other's species
                bool involves(const Components &) const noexcept; //!< this hiread one of components
                void extract(const EList &);

                //! check is and how much broken is the law
                /**
                 \param  gain positive if broken
                 \param  Cout output corrected concentration MUST be Cinp
                 \param  Lout output level
                 \param  Cinp input broken (or not) concentration
                 \param  Linp input level
                 \param  xadd to perform internal additions
                 \return true if broken
                 */
                bool    broken(xreal_t &         gain,
                               XWritable &       Cout,
                               const Level       Lout,
                               const XReadable & Cinp,
                               const Level       Linp,
                               XAdd             &xadd) const;

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                const xreal_t  xden; //!< |nu|^2
                const XMatrix  proj; //!< in group projection
                const EList    base; //!< primary involved equilibri[um|a]
                Law *          next; //!< for list
                Law *          prev; //!< for list

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Law);
                
            };
        }
    }
}


#endif

