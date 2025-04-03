
//! \file

#ifndef Y_Chemical_Conservation_Broken_Included
#define Y_Chemical_Conservation_Broken_Included 1

#include "y/chemical/plexus/conservation/law.hpp"
#include "y/chemical/type/assembly.hpp"

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
            //! Broken Law and mending solution
            //
            //
            //__________________________________________________________________
            class Broken
            {
            public:
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                Broken( const Law &   _law,
                       const xreal_t _xs,
                       XWritable &   _cc) noexcept; //!< setup
                ~Broken()                 noexcept; //!< cleanup
                Broken(const Broken &_)   noexcept; //!< duplicate

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

                //! pretty show
                std::ostream & show(std::ostream &os, const Assembly &assembly) const;

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                const Law & law; //!< the law
                xreal_t     xs;  //!< excess
                XWritable & cc;  //!< AuxLevel projection

            private:
                Y_DISABLE_ASSIGN(Broken);
            };

            typedef Small::SoloHeavyList<Broken> BList; //!< alias
            typedef BList::NodeType              BNode; //!< alias
        }
    }

}

#endif

