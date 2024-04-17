
#include "y/chemical/species/library.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        const char * const Library:: CallSign = "Chemical::Library";

        Library:: ~Library() noexcept
        {
        }

        Library:: Library() : sdb()
        {
        }

        Library::ConstInterface & Library:: surrogate() const noexcept { return sdb; }

        void Library:: throwChargeMismatch(const String &name) const
        {
            throw Specific::Exception(CallSign, "charge mismatch for '%s'", name.c_str());
        }

        void Library:: throwFailedToInsert(const String &name) const
        {
            throw Specific::Exception(CallSign, "unexpected failed to insert '%s'", name.c_str());
        }

    }

}


