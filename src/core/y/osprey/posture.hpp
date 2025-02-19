
//! \file


#ifndef Y_Osprey_Posture_Included
#define Y_Osprey_Posture_Included 1


#include "y/osprey/content.hpp"
#include "y/osprey/residue.hpp"

namespace Yttrium
{
    namespace Osprey
    {

        class Posture
        {
        public:
            explicit Posture(const IBank &bank,
                             const size_t dims,
                             const size_t excl);

            explicit Posture(const Posture &     root,
                             const INode * const node);


            virtual ~Posture() noexcept;

            Y_OSTREAM_PROTO(Posture);

            static bool AreEqual(const Posture &lhs, const Posture &rhs) noexcept;

            //! try remove in content or residue
            bool tryRemove(const size_t indx) noexcept;

            //! tranfer residue to content
            void flush() noexcept;



            Content      content;
            Residue      residue;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Posture);
        };
    }

}

#endif

