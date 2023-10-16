
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
            fp << ']';
            Endl(fp);

            for(const Actor *a=reac.head;a;a=a->next)
            {
                Arrow(fp, &(a->sp), this);
                if(a->nu>1)
                {
                    fp("[label=\"%u\"]",a->nu);
                }
                Endl(fp);
            }

            for(const Actor *a=prod.head;a;a=a->next)
            {
                Arrow(fp, this, &(a->sp) );
                if(a->nu>1)
                {
                    fp("[label=\"%u\"]",a->nu);
                }
                Endl(fp);
            }
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
