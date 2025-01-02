

#ifndef Y_Chemical_Conservation_Warden_Included
#define Y_Chemical_Conservation_Warden_Included 1

#include "y/chemical/plexus/conservation/law.hpp"
#include "y/chemical/type/assembly.hpp"

#include "y/stream/xmlog.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Mix;

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
            class Warden : public Assembly
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                static const char * const CallSign; //!< "Chemical::Warden"

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________

                //! setup
                explicit Warden(const Mix & _mix, const Act & _act);
                virtual ~Warden() noexcept; //!< cleanup

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________
                void run(XMLog &xml,XWritable &C, const Level L);

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                const Mix & mix;   //!< persistent mix
                const Act & act;   //!< persistent act in mix
                XAdd        xadd;  //!< eXtended addition



            private:
                Y_DISABLE_COPY_AND_ASSIGN(Warden);
            };
        }

    }

}

#endif

