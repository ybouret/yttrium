#include "y/chemical/reactive/equilibria.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        const char * const Equilibria:: CallSign = "Chemical::Equilibria";

        Equilibria:: Equilibria() : edb()
        {
        }

        Equilibria:: ~Equilibria() noexcept
        {
        }

        

        Equilibria::ConstInterface & Equilibria:: surrogate() const noexcept { return edb; }
   

        void Equilibria:: insert(const Equilibrium::Handle &handle)
        {
            if(!edb.insert(handle)) throw Specific::Exception(CallSign, "multiple <%s>", handle->name.c_str());
            updateWith(*handle);
        }

        size_t Equilibria:: topLevel() const noexcept { return edb.size() + 1; }


        std::ostream & operator<<(std::ostream &os, const Equilibria &eqs)
        {
            os << "<" << Equilibria::CallSign << " size='" << eqs->size() << "'>" << std::endl;

            for(Equilibria::ConstIterator it=eqs->begin();it!=eqs->end();++it)
            {
                const Equilibrium &eq = **it;
                eqs.display(os << "  ",Coerce(eq));
                //eqs.pad(os << "\t<" << eq.name << ">",eq) << Equilibrium::Separator;
                //eq.showEquilibrium(os << "\t", eqs.maxLength,eqs.rmax,eqs.pmax) << std::endl;
                //eqs.print(os,eq) << Equilibrium::Separator;
                os << std::endl;
            }

            os << "<" << Equilibria::CallSign << "/>";
            return os;
        }

        void Equilibria:: finalize()
        {
            for(Equilibria::Iterator it=edb.begin();it!=edb.end();++it)
            {
                const Equilibrium &eq = **it;
                const char * const id = eq.name.c_str(); if(eq->size()<=0) throw Specific::Exception(CallSign,"empty <%s>", id);
                const int          dz = eq.charge();     if(0!=dz)         throw Specific::Exception(CallSign, "<%s> charge is %d", id, dz);
                modernizeWith(eq);
            }
        }


    }

}
