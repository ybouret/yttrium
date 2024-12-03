#include "y/chemical/reactive/components.hpp"
#include "y/system/exception.hpp"
#include <cerrno>
#include "y/kemp/integer.hpp"

namespace Yttrium
{
    namespace Chemical
    {

#define Y_ChemCm(PPTY) case PPTY: return #PPTY

        const char * Components:: AttributeText(const Attribute p) noexcept
        {
            switch(p)
            {
                    Y_ChemCm(Nebulous);
                    Y_ChemCm(ReacOnly);
                    Y_ChemCm(ProdOnly);
                    Y_ChemCm(Definite);
            }
            return Core::Unknown;
        }

        Components:: ~Components() noexcept
        {
        }

        Components:: ConstInterface & Components:: surrogate() const noexcept { return cmdb; }


        const String & Components:: key() const noexcept
        {
            return name;
        }

      
        void Components :: viz(OutputStream &fp,
                               const char * const color,
                               const char * const style) const
        {
            Node(fp,this) << '[';
            Label(fp,name) << ",shape=box3d";
            if(color) fp << ",color=" << color << ",fontcolor=" << color;
            if(style) fp << "style=\"" << style << "\"";
            Endl(fp<<']');
            
            for(ConstIterator it=cmdb.begin();it!=cmdb.end();++it)
            {
                const Component &cm = *it;
                cm.viz(fp,*this,color);
            }

        }

        void Components:: graphViz(OutputStream &fp) const
        {
            Enter(fp, "G");
            for(ConstIterator it=cmdb.begin();it!=cmdb.end();++it)
            {
                const Component &cm = *it;
                cm.actor.sp.viz(fp,0,0);
            }
            viz(fp,0,0);
            Leave(fp);
        }

      

      


        const char * Components:: attrText() const noexcept
        {
            return AttributeText(attr);
        }



       

        


        void Components:: addSpeciesTo(AddressBook &book) const
        {
            for(ConstIterator it=cmdb.begin();it!=cmdb.end();++it)
            {
                book |= (*it).actor.sp;
            }
        }

        const char * const Components::Scheme = "set18";

        String Components:: makeColor() const
        {
            return makeColorFrom(Scheme);
        }


    }

}

#include "y/chemical/weasel.hpp"

namespace Yttrium
{

    namespace Chemical
    {

        std::ostream & Components:: print(std::ostream &os) const
        {

            os << name << Weasel::COLON << reac << ' ' << Weasel::SYMBOL  << ' ' << prod;

            return os;
        }


    }

}

