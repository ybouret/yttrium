#include "y/chem/reactive/equilibria.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Equilibria:: Equilibria() : db(), maxReac(), maxProd() {}

        Equilibria:: ~Equilibria() noexcept
        {

        }

        Equilibria::ConstInterface & Equilibria:: surrogate() const noexcept
        {
            return db;
        }

        const char * const Equilibria::CallSign = "Chemical::Equilibria";

        void Equilibria:: submit(Equilibrium *eq)
        {
            assert(0!=eq);

            // try to insert it in database
            {
                const Equilibrium::Pointer ep(eq);
                if(!db.insert(ep))
                    throw Specific::Exception(CallSign,"multiple <%s>", eq->name.c_str());
            }

            // initial indices
            const size_t idx = db.size();
            for(unsigned i=0;i<Levels;++i) Coerce(eq->indx[i]) = idx;

            // for format
            updateWith(*eq);
        }

        void Equilibria:: updateMaxSizes() noexcept
        {
            for(Iterator it=db.begin();it!=db.end();++it)
            {
                const Components &eq = **it;
                {
                    const String s = eq.reac.toString();
                    Coerce(maxReac).updateWith(s.size());
                }
                {
                    const String s = eq.prod.toString();
                    Coerce(maxProd).updateWith(s.size());
                }
            }
        }



        std::ostream & operator<<(std::ostream &os, const Equilibria &eqs)
        {
            os << '{' << std::endl;
            for(Equilibria::ConstIterator it=eqs->begin();it!=eqs->end();++it)
            {
                const Equilibrium &eq = **it;
                eqs.pad(os << "  <" << eq.name << ">",eq) << " : ";
                {
                    const String s = eq.reac.toString();
                    eqs.maxReac.pad(os << s,s.size());
                }
                os << " <=> ";
                {
                    const String s = eq.prod.toString();
                    eqs.maxProd.pad(os << s,s.size());
                }
                os << " : ";
                os << std::endl;
            }
            os << '}';
            return os;
        }
    }

}

