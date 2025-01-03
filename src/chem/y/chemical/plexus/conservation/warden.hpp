

#ifndef Y_Chemical_Conservation_Warden_Included
#define Y_Chemical_Conservation_Warden_Included 1

#include "y/chemical/plexus/conservation/law.hpp"
#include "y/chemical/type/assembly.hpp"
#include "y/data/small/heavy/list/solo.hpp"
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
            //! Broken law status
            //
            //
            //__________________________________________________________________
            class Broken {
            public:
                Broken(const Law  & _law,
                       const xReal  _bad,
                       XWritable  & _cok) noexcept :
                law(_law),
                bad(_bad),
                cok(_cok)
                {
                }

                ~Broken() noexcept {}

                Broken(const Broken &_) noexcept :
                law(_.law),
                bad(_.bad),
                cok(_.cok)
                {
                }

                bool fixed(XAdd &xadd, const XReadable &Cin) {
                    bad = law.excess(xadd, Cin, AuxLevel);
                    if(bad>0.0)
                    {
                        law.project(xadd,cok,Cin,AuxLevel);
                        return false;
                    }
                    else
                    {
                        return true;
                    }
                }




                const Law & law;
                xReal       bad;
                XWritable  &cok;

            private:
                Y_DISABLE_ASSIGN(Broken);
            };

            typedef Small::SoloHeavyList<Broken> BList;
            typedef BList::NodeType              BNode;

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
            private:
                const Mix &  mix;      //!< persistent mix
                const Act &  act;      //!< persistent act in mix
                XAdd         xadd;     //!< eXtended addition
                XAdds        Cadd;     //!< sum of added concentrations
                XArray       Cini;     //!< temporary
                XMatrix      Corr;     //!< corrected concentrations
                BList        blist;    //!< list of broken laws in act
            public:
                const XArray extra;    //!< what was added

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Warden);
            };
        }

    }

}

#endif

