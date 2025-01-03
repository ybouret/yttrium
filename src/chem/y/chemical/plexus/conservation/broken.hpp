
//! \file

#ifndef Y_Chemical_Conservation_Broken_Included
#define Y_Chemical_Conservation_Broken_Included 1

#include "y/chemical/plexus/conservation/law.hpp"
#include "y/data/small/heavy/list/solo.hpp"

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
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________

                //! setup with bad excess
                Broken(const Law  & _law,
                       const xReal  _bad,
                       XWritable  & _cok) noexcept;
                ~Broken()                 noexcept; //!< cleanup
                Broken(const Broken &)    noexcept; //!< duplicate

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

                //! check if Cin fixed the law
                bool fixed(XAdd &xadd, const XReadable &Cin);

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                const Law &  law; //!< law
                xReal        bad; //!< bad excess
                XWritable  & cok; //!< corrected concentration

            private:
                Y_DISABLE_ASSIGN(Broken);
            };

            typedef Small::SoloHeavyList<Broken> BList; //!< alias
            typedef BList::NodeType              BNode; //!< alias

        }

    }

}

#endif

