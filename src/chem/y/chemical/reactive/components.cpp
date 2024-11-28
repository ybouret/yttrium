#include "y/chemical/reactive/components.hpp"
#include "y/system/exception.hpp"
#include <cerrno>

namespace Yttrium
{
    namespace Chemical
    {
        Components:: ~Components() noexcept
        {
        }

        Components:: ConstInterface & Components:: surrogate() const noexcept { return cmdb; }


        const String & Components:: key() const noexcept
        {
            return name;
        }

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

        }

        void Components:: operator()(const Role role, const Species &sp)
        {
            (*this)(role,1,sp);
        }

        void Components :: viz(OutputStream &fp,
                               const char * const color,
                               const char * const style) const
        {
            Node(fp,this) << '[';
            Label(fp,name) << ",shape=box3d";
            if(color) fp << "color=\"" << color << "\"";
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

