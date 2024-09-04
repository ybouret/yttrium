

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

            //! return Cws as Cin*(1-u) + Cex*u, clamped
            const XReadable &probe(const xreal_t u);

            //! objFunc( probe(u), SubLevel )
            xreal_t operator()(const xreal_t u);

            void basisToRate(XWritable &rate);


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

            void showProspects(XMLog &xml, const XReadable &Ktop) const;

        };
    }

}


#endif
