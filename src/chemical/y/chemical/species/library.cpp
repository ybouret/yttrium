
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



        const Species & Library:: manage(const Species::Handle &handle)
        {
            const String         &target = handle->name;
            {
                const Species::Handle * const query = sdb.search(target);
                if(query)
                {
                    const Species &mine = **query;
                    if(mine.z != handle->z)  throw Specific::Exception(CallSign, "charge mismatch for '%s'", target.c_str());
                    return mine;
                }
            }
            assert(0==sdb.search(target));
            if(!sdb.insert(handle))   throw Specific::Exception(CallSign, "unexpected failed to insert '%s'", target.c_str());
            updateWith(*handle);
            return *handle;
        }

    }

}


