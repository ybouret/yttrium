
#include "y/chemical/reactive/equilibrium.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Equilibrium:: ~Equilibrium() noexcept
        {
        }
        
        XReal Equilibrium:: K(Real t)
        {
            static const XReal _0(0);
            const XReal k = getK(t);
            if(k<=_0) throw Specific::Exception("Chemical::Equilibrium", "%s.K=%g at t=%g", name.c_str(), double(k), double(t) );
            return k;
        }

        const char * const Equilibrium::Colors = "set19";


        void    Equilibrium:: vizColor(OutputStream &fp) const
        {
            Color(fp << "color=",     Colors, indx[SubLevel]);
            Color(fp << ",fontcolor=", Colors, indx[SubLevel]);
        }

        void Equilibrium:: viz(OutputStream &fp) const
        {
            Node(fp,this) << '[';
            Label(fp,name) << ",shape=box,";
            vizColor(fp);
            Endl(fp<<']');
        }



        void Equilibrium:: vizLink(OutputStream &fp) const
        {
            for(const Actor *a=reac.head;a;a=a->next)
            {
                Arrow(fp,&(a->sp),this) << '[';
                if(a->nu>1)
                {
                    fp("label=\"%u\",",unsigned(a->nu));
                }
                vizColor(fp);
                Endl(fp << ']');
            }

            for(const Actor *a=prod.head;a;a=a->next)
            {
                Arrow(fp,this,&(a->sp)) << '[';
                if(a->nu>1)
                {
                    fp("label=\"%u\",",unsigned(a->nu));
                    vizColor(fp);
                }
                Endl(fp << ']');
            }

        }




    }

}
