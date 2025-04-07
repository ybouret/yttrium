//! \file

#ifndef Y_Chemical_Conservation_Law_Included
#define Y_Chemical_Conservation_Law_Included 1

#include "y/chemical/reactive/actors.hpp"
#include "y/chemical/type/defs.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Conservation
        {
            class Rule;

            //__________________________________________________________________
            //
            //
            //
            //! Law and its data built from rule
            //
            //
            //__________________________________________________________________
            class Law :  public Actors, public GraphViz::Vizible
            {
            public:
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit Law(Rule * const); //!< setup and latch
                virtual ~Law() noexcept;    //!< cleanup

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

                //! emit GraphViz code once species are emitted
                void    viz(OutputStream &fp, const String * const color=0) const;

                //! compute POSITIVE excess, Zero meaning law is followed
                xreal_t excess(XAdd &xadd, const XReadable &C, const Level L) const;

                

                //! check if common actor(s)
                bool hasCommonActorWith(const Law &other) const noexcept;

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                const unsigned norm2; //!< |*this|^2
                const xreal_t  denom; //!< norm2
                const size_t   auxId; //!< in canon
                const size_t   subId; //!< identifier in cluster
                Law *          next;  //!< for list
                Law *          prev;  //!< for list
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Law);
            };

        }
    }
}
#endif
