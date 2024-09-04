

//! \file

#ifndef Y_Chemical_Solver_Included
#define Y_Chemical_Solver_Included 1


#include "y/chemical/plexus/solver/prospect.hpp"
#include "y/chemical/plexus/joint.hpp"
#include "y/chemical/reactive/aftermath.hpp"
#include "y/mkl/algebra/lu.hpp"
#include "y/orthogonal/family.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Solver : public Joint
        {
        public:

            explicit Solver(const Cluster &);
            virtual ~Solver() noexcept;


            
            void     process(XWritable &C, const Level L, const XReadable &Ktop, XMLog &xml);
            xreal_t  objFunc(const XReadable &C, const Level L);
            void     nrStage(XWritable &C, const Level L, XMLog &xml);
            void     odeStep(XWritable &C, const Level L, XMLog &xml);

            const XReadable &probe(const xreal_t u)
            {
                const xreal_t one = 1;
                const xreal_t v   = one-u;
                XWritable    &C   = Cws;
                for(size_t j=nspc;j>0;--j)
                {
                    const xreal_t c0 = Cin[j];
                    const xreal_t c1 = Cex[j];
                    xreal_t cmin=c0, cmax=c1;
                    if(cmax<cmin) Swap(cmin,cmax);
                    C[j] = Clamp(cmin, c0*v + c1*u, cmax);
                }
                return C;
            }

            void basisToRate(XWritable &rate)
            {
                // initialize inc
                inc.forEach( &XAdd::free );

                // collect of increases from current basis
                for(const PNode *pn=basis.head;pn;pn=pn->next)
                    (**pn).step(inc);

                // deduce rate
                for(const SNode *sn=mine.species.head;sn;sn=sn->next)
                {
                    const size_t j = (**sn).indx[ SubLevel ];
                    rate[j] = inc[j].sum();
                }

            }


            xreal_t operator()(const xreal_t u)
            {
                return objFunc( probe(u), SubLevel);
            }


            Aftermath          afm;
            XMatrix            ceq;
            Prospect::Series   pps;
            const size_t       dof;
            XSeries            obj;
            Orthogonal::Family ortho;
            PBank              pbank;
            PRepo              basis;
            XArray             Cin;
            XArray             Cex;
            XArray             Cws;
            XArray             ddC;
            XSwell             inc;
            MKL::LU<xreal_t>   xlu;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Solver);

            void showProspects(XMLog &xml, const XReadable &Ktop) const
            {
                if(xml.verbose)
                {
                    for(size_t i=1;i<=pps.size();++i)
                    {
                        pps[i].show( xml(), mine, Ktop) << std::endl;
                    }
                }
            }



        };
    }

}


#endif
