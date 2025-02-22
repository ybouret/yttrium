//! \file

#ifndef Y_Osprey_Posture_Included
#define Y_Osprey_Posture_Included 1

#include "y/osprey/content.hpp"
#include "y/osprey/residue.hpp"

namespace Yttrium
{
    namespace Osprey
    {
        //______________________________________________________________________
        //
        //
        //
        //! Content+Residue
        //
        //
        //______________________________________________________________________
        class Posture
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup with dimensions and first index (excluded from residue)
            explicit Posture(const IBank &bank,
                             const size_t dims,
                             const size_t excl);

            //! duplicate/upgrade
            explicit Posture(const Posture &     root,
                             const INode * const node);

            virtual ~Posture() noexcept; //!< cleanup
            Y_OSTREAM_PROTO(Posture);    //!< display

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! check equal contents and equivalent residues
            static bool AreEqual(const Posture &lhs, const Posture &rhs) noexcept;

            //! try remove in content or residue
            bool tryRemove(const size_t indx) noexcept;

            //! tranfer residue to content
            void flush() noexcept;


            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            Content      content; //!< content (indices used to build family)
            Residue      residue; //!< residue (indices available)

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Posture);
        };
    }

}

#endif

