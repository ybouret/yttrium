
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

        const char *              Library:: callSign()  const noexcept { return CallSign; }
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


        std::ostream & operator<<(std::ostream &os, const Library &lib)
        {
            os << "<" << lib.callSign() << " species='" << lib->size() << "'>" << std::endl;
            for(Species::Set::ConstIterator it=lib->begin(); it != lib->end(); ++it)
            {
                const Species &sp = **it;
                lib.pad(std::cerr << "\t" << sp, sp) << " : z = " << std::setw(4) << sp.z << std::endl;
            }
            os << "<" << lib.callSign() << "/>";
            return os;
        }


        const Species & Library:: operator[](const String &id) const
        {
            const Species::Handle *pps = sdb.search(id);
            if(!pps) throw Specific::Exception(CallSign,"unknown '%s'", id.c_str());
            return **pps;
        }

        const Species & Library:: operator[](const char *id) const
        {
            const String _(id);
            return (*this)[_];
        }


    }

}


