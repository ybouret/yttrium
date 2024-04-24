
#include "y/chemical/reactive/equilibrium.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Equilibrium:: ~Equilibrium() noexcept
        {
        }
        
        xreal_t Equilibrium:: K(real_t t)
        {
            static const xreal_t _0(0);
            const xreal_t k = getK(t);
            if(k<=_0) throw Specific::Exception("Chemical::Equilibrium", "%s.K=%g at t=%g", name.c_str(), double(k), double(t) );
            return k;
        }

        const char * const Equilibrium::Colors = "dark28";

        const String Equilibrium:: vizColor(const Level level) const
        {
            const String descr = Color(Colors,indx[level]);
            return "color="+descr+",fontcolor="+descr;
        }



        void Equilibrium:: viz(OutputStream &fp, const Level level) const
        {
            Node(fp,this) << '[';
            Label(fp,name) << ",shape=box,";
            fp << vizColor(level);
            Endl(fp<<']');
        }



        void Equilibrium:: vizLink(OutputStream &fp, const Level level) const
        {
            const String c = vizColor(level);
            for(const Actor *a=reac.head;a;a=a->next)
            {
                Arrow(fp,&(a->sp),this) << '[';
                if(a->nu>1)
                {
                    fp("label=\"%u\",",unsigned(a->nu));
                }
                fp << c;
                Endl(fp << ']');
            }

            for(const Actor *a=prod.head;a;a=a->next)
            {
                Arrow(fp,this,&(a->sp)) << '[';
                if(a->nu>1)
                {
                    fp("label=\"%u\",",unsigned(a->nu));
                }
                fp << c;
                Endl(fp << ']');
            }

        }




    }

}
