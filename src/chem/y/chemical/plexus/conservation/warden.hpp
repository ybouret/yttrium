//! \file

#ifndef Y_Chemical_Conservation_Warden_Included
#define Y_Chemical_Conservation_Warden_Included 1

#include "y/chemical/plexus/conservation/broken.hpp"
#include "y/chemical/type/assembly.hpp"
#include "y/data/small/heavy/list/solo.hpp"
#include "y/stream/xmlog.hpp"

namespace Yttrium
{
    namespace Chemical
    {


        namespace Conservation
        {

            class Act;

            //__________________________________________________________________
            //
            //
            //
            //! Apply single act
            //
            //
            //__________________________________________________________________
            class Warden : public Oversized, public Assembly
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                static const char * const CallSign; //!< "Chemical::Warden"
                typedef CxxListOf<Warden> List;     //!< alias
                
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________

                //! setup
                explicit Warden(const Act & _act);

                //! cleanup
                virtual ~Warden() noexcept;

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

                //! make legal concentrations according to act
                /**
                 - on output,  injected (at Level L) is computed
                 */
                void run(XMLog &xml, XWritable &C, const Level L, XWritable &injected);

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
            private:
                const Act &  act;      //!< persistent act in mix
                XAdd         xadd;     //!< eXtended addition
                XAdds        Cadd;     //!< sum of added concentrations
                XArray       Cini;     //!< temporary
                XMatrix      Corr;     //!< corrected concentrations
                BList        blist;    //!< list of broken laws in act
            public:
                Warden *     next;     //!< for list
                Warden *     prev;     //!< for list

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Warden);
            };
        }

    }

}

#endif

