

//! \file

#ifndef Y_Chemical_Solver_Included
#define Y_Chemical_Solver_Included 1

#include "y/chemical/plexus/solver/vertices.hpp"
#include "y/chemical/plexus/solver/prospect.hpp"
#include "y/chemical/plexus/joint.hpp"
#include "y/chemical/reactive/aftermath.hpp"
#include "y/mkl/algebra/lu.hpp"
#include "y/orthogonal/family.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        

      
        //______________________________________________________________________
        //
        //
        //
        //! Solver for a cluster with valid concentrations
        //
        //
        //______________________________________________________________________
        class Solver : public Joint
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            static const char * const CallSign; //!< definitions
            static const char * const NRA_Step; //!< "nrastep"
            static const char * const ODE_Step; //!< "odestep"
            static const char * const MIN_Step; //!< "minstep"

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Solver(const Cluster &); //!< setup
            virtual ~Solver() noexcept;       //!< cleanup


            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________




            //! upgrade and apply strategy
            void  process(XWritable &C, const Level L, const XReadable &Ktop, XMLog &xml);


            void  run(XWritable &C, const Level L, const XReadable &Ktop, XMLog &xml);



            //! objective function over prospects
            xreal_t  objFunc(const XReadable &C, const Level L);

            //! object function+gradient (in grd) over prospects
            xreal_t  objGrad(const XReadable &C, const Level L);
            
            //! return Cws as Cin*(1-u) + Cex*u, clamped
            const XReadable &probe(const xreal_t u);

            //! objFunc( probe(u), SubLevel )
            xreal_t operator()(const xreal_t u);

            //! compute target from source+deltaC
            /**
             deltaC is scaled to guarantee valid target
             \param target output valid concentrations
             \param source input valid concentrations
             \param deltaC input and output deltaC
             \param result optional scaling factor
             */
            bool stepWasCut(XWritable &       target,
                            const XReadable & source,
                            XWritable &       deltaC,
                            xreal_t * const   result) const;

            //! compute Rate from selected prospects dC
            void computeRate(XWritable &rate);

            //! check all basis eqs are ok with given concentrations
            bool basisOkWith(const XReadable &C, const Level L) const noexcept;
        
            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            Aftermath          afm;   //!< compute prospects
            XMatrix            ceq;   //!< cc memory
            XMatrix            deq;   //!< dc memory
            Prospect::Series   pps;   //!< current prospects
            const size_t       dof;   //!< number of original eqs
            XSeries            obj;   //!< helper for objFunc
            Orthogonal::Family ortho; //!< helper to find basis
            PBank              pbank; //!< memory for basis
            PRepo              basis; //!< basis of leading eqs
            Vertices           vlist; //!< list of final candidates
            XArray             Cin;   //!< C init for lookup
            XArray             Cex;   //!< C exit for lookup
            XArray             Cws;   //!< C workspace
            XArray             ddC;   //!< deltaC
            Solver            &fcn;   //!< *this, to call fcn(...)
            xreal_t            ff0;   //!< initial value
            XArray             grd;   //!< initial gradient
            XSwell             inc;   //!< increases
            MKL::LU<xreal_t>   xlu;   //!< linear solver
            const xreal_t      xsf;   //!< xreal safe factor = 0.99


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Solver);

            void showProspects(XMLog &xml, const XReadable &Ktop) const;
            void saveProfile(const String   &fn,  const size_t np); //!< assuming Cin and Cex are set
            void saveProfile(const Prospect &pro, const size_t np); //!< assuming Cin is set, set Cex as pro.cc

            bool nraStep(XMLog &xml); //!< successful Newton-Raphson Algorithm step
            bool odeStep(XMLog &xml); //!< successful ODE-like step
            bool located(const char * const fn, XMLog &xml); //!< locate min of objective function in Cin:Cex, true if decreases ff0

            //! solve crucial, collect prospects and basis
            void     upgrade(XWritable &C, const Level L, const XReadable &Ktop, XMLog &xml);

            
        };
     }

}


#endif
