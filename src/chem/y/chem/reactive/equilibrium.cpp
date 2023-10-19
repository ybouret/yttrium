
#include "y/chem/reactive/equilibrium.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Equilibrium:: ~Equilibrium() noexcept {}

        xreal Equilibrium:: K(double t)
        {
            const xreal k = getK(t);
            if(k.mantissa<=0) throw Specific:: Exception( name.c_str(), " K(%g)<=0",t);
            return getK(t);
        }

        std::ostream & operator<<(std::ostream &os, const Equilibrium &eq)
        {
            return os << '<' << eq.name << '>';
        }

        const char * const Equilibrium:: SchemeId = "dark27";

        void Equilibrium:: vizColor(OutputStream &fp) const
        {
            Color(fp << "color=",     SchemeId, indx[SubLevel]);
            Color(fp << ", fontcolor=", SchemeId, indx[SubLevel]);
        }

        void Equilibrium:: vizArrow(OutputStream &fp, const Vizible *v, const bool to, const unsigned nu) const
        {
            assert(0!=v);
            if(to)
            {
                Arrow(fp,this,v);
            }
            else
            {
                Arrow(fp,v,this);
            }
            fp << '[';
            vizColor(fp);
            if(nu>1)
            {
                fp(", label=\"%u\"",nu);
            }
            fp << ']';
            Endl(fp);
        }


        void Equilibrium:: viz(OutputStream &fp) const
        {
            Node(fp,this);
            fp << '[';
            Label(fp,name);
            const char *shape = "box";
            if(reac.size<=0)
                shape = "invtrapezium";
            if(prod.size<=0)
                shape = "trapezium";
            fp << ", shape=" << shape;
            vizColor(fp<<", ");

            fp << ']';
            Endl(fp);

            for(const Actor *a=reac.head;a;a=a->next)
                vizArrow(fp, &(a->sp), false, a->nu);

            for(const Actor *a=prod.head;a;a=a->next)
                vizArrow(fp, &(a->sp), true, a->nu);

        }

        void Equilibrium:: graphViz(OutputStream &fp) const
        {
            Enter(fp,name);

            for(const Actor *a=reac.head;a;a=a->next)
            {
                a->sp.viz(fp);
            }

            for(const Actor *a=prod.head;a;a=a->next)
            {
                a->sp.viz(fp);
            }

            viz(fp);

            Leave(fp);
        }

    }

}

namespace Yttrium
{
    namespace Chemical
    {

        ConstEquilibrium:: ~ConstEquilibrium() noexcept {}

        xreal ConstEquilibrium:: getK(double)
        {
            return K_;
        }


    }

}
