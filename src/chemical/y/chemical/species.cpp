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


        void Species:: viz(OutputStream &       fp,
                           const String * const color    ,
                           const bool           conserved) const
        {
            Node(fp,this) << '[';

            fp << "label=< ";
            fp << *html;
            fp << " >";

            fp << ",shape=ellipse";
            if(!conserved) fp << ",style=dashed";
            if(color)      fp << ',' << *color;
            Endl(fp<<']');
        }

    }
}


