

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
        

        class Vertex : public Quantized, public XArray
        {
        public:
            typedef CxxListOf<Vertex> List;
            typedef CxxPoolOf<Vertex> Pool;

            explicit Vertex(const size_t m) :
            Quantized(),
            XArray(m),
            cost(),
            next(0),
            prev(0)
            {
            }

            virtual ~Vertex() noexcept
            {
            }


            xreal_t cost; //!< whatever cost
            Vertex *next; //!< for list/pool
            Vertex *prev; //!< for list

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Vertex);
        };

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



            //! solve crucial, collect prospects and basis
            void     upgrade(XWritable &C, const Level L, const XReadable &Ktop, XMLog &xml);

            //! upgrade and apply strategy
            void     process(XWritable &C, const Level L, const XReadable &Ktop, XMLog &xml);

            xreal_t  objFunc(const XReadable &C, const Level L);
            xreal_t  objGrad(const XReadable &C, const Level L);
            
            //! return Cws as Cin*(1-u) + Cex*u, clamped
            const XReadable &probe(const xreal_t u);

            //! objFunc( probe(u), SubLevel )
            xreal_t operator()(const xreal_t u);

            bool stepWasCut(XWritable &       target,
                            const XReadable & source,
                            XWritable &       deltaC,
                            xreal_t * const   result) const;


            void computeRate(XWritable &rate);
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
            Vertex::List       vlist; //!< list of final candidates
            Vertex::Pool       vpool; //!< pool of final candidates
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

            bool nraStep(XMLog &xml);
            bool odeStep(XMLog &xml);

        };
     }

}


#endif
