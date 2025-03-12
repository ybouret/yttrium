#include "y/chemical/species.hpp"


namespace Yttrium
{
    namespace Chemical
    {
        Species:: ~Species() noexcept
        {
        }

        std::ostream & operator<<(std::ostream &os, const Species &sp)
        {
            os << sp.key();
            return os;
        }

        size_t Species:: serialize(OutputStream &fp) const
        {
            return name->serialize(fp);
        }

        Species * Species:: ReadFrom(InputStream &fp,const size_t id)
        {
            const String description = String::ReadFrom(fp,Formula::CallSign);
            return new Species(description,id);
        }
    }
}


