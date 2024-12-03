
#include "y/chemical/reactive/components.hpp"
#include "y/system/exception.hpp"
#include <cerrno>
#include "y/kemp/integer.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        
        Actors & Components:: actorsPlaying(const Role role)
        {
            switch(role)
            {
                case Reactant: return Coerce(reac);
                case Product:  return Coerce(prod);
            }
            throw Libc::Exception(EINVAL, "invalid Chemical::Role!!");
            // never get here
        }

        void Components:: operator()(const Role role, const unsigned nu, const Species &sp)
        {
            const String &sid = sp.name;

            // check coef
            if(nu<=0) throw Specific::Exception(name.c_str(), "null stoichiometry for '%s'", sid.c_str());

            // check that species is not used
            {
                const Component * const cm = cmdb.search(sid);
                if(0!=cm)
                    throw Specific::Exception(name.c_str(), "already used as %s %s", cm->actor.name.c_str(), cm->side());
            }

            // get concerned actors
            Actors &      actors = actorsPlaying(role);
            Actors        backup = actors;

            // create new actor
            const Actor & actor  = actors(nu,sp);

            // safe insertion
            try {
                const Component cm(actor,role);
                if(!cmdb.insert(cm))
                    throw Specific::Exception(name.c_str(),"couldn't insert %s %s", cm.actor.name.c_str(), cm.side());
            }
            catch(...)
            {
                actors.company.xch(backup.company);
                throw;
            }

            switch(role)
            {
                case Product:  Coerce(d_nu) += int(nu); break;
                case Reactant: Coerce(d_nu) -= int(nu); break;
            }

            updateAttribute();
        }

        void Components:: operator()(const Role role, const Species &sp)
        {
            (*this)(role,1,sp);
        }


        void Components:: updateAttribute() noexcept
        {
            if(reac->size>0)
            {
                if(prod->size>0)
                {
                    Coerce(attr) = Definite;
                }
                else
                {
                    Coerce(attr) = ReacOnly;
                }
            }
            else
            {
                if(prod->size>0)
                {
                    Coerce(attr) = ProdOnly;
                }
                else
                {
                    Coerce(attr) = Nebulous;
                }
            }
        }


        static inline void sumZ(apz &sum, const Actor *a)
        {
            for(;a;a=a->next)
            {
                apz z = a->sp.z;
                z *= a->nu;
                sum += z;
            }
        }

        bool Components:: neutral() const
        {
            apz lhs = 0; sumZ(lhs,reac->head);
            apz rhs = 0; sumZ(rhs,prod->head);
            return lhs == rhs;
        }

        void Components:: applicable() const
        {
            const char * const id = name.c_str();
            if(!neutral() )    throw Specific::Exception(id, "charge is not conserved");
            if(Nebulous==attr) throw Specific::Exception(id, "no components");
        }


    }

}

