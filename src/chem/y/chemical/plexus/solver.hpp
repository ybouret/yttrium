
//! \file

#ifndef Y_Chemical_Solver_Included
#define Y_Chemical_Solver_Included 1

#include "y/chemical/plexus/solver/engine.hpp"
#include "y/chemical/plexus/type/cursors.hpp"

#include "y/chemical/reactive/equilibrium/aftermath.hpp"
#include "y/orthogonal/family.hpp"
#include "y/mkl/algebra/lu.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Mix;

        //______________________________________________________________________
        //
        //
        //
        //! Mix solver
        //
        //
        //______________________________________________________________________
        class Solver : public Aftermath, public Proxy<const Engine>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const CallSign;        //!< "Chemical::Solver"
            static const char * const OptimalProspect; //!< "OptimalProspect"
            static const char * const DynamicalSystem; //!< "DynamicalSystem"
            static const char * const AlgebraicSystem; //!< "AlgebraicSystem"

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Solver(const Mix    &,
                            const CrBank &,
                            const SBank  &); //!< setup from persistent mix
            virtual ~Solver() noexcept;      //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! solve concentration with globa K
            void run(XMLog &xml, XWritable &C, const Level L, const XReadable &K);

            //! objective function at Cini * (1-u) + u * Cend
            xReal operator()(const xReal u);

            //! check that all prospects accept C
            bool isAcceptable(const XReadable &C, const Level L) const noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Solver);
            Y_PROXY_DECL();

            xReal                    objectiveFunction(const XReadable &C, const Level L);
            xReal                    objectiveFunction(const xReal u);
            xReal                    computeStepDS(XMLog &xml, const xReal f0);
            void                     setRecentStep(const xReal scal) noexcept; //!< Cend = Cini + scal * step
            void                     getOrthoBasis(XMLog &xml);
            xReal                    computeStepAS(XMLog &xml, const xReal f0);

            void                     save(OutputStream &fp, const size_t np);    //!< save with Cini/Cend loaded
            void                     save(const String &fn, const size_t np);    //!< save with Cini/Cend loaded
            void                     savePro(const String &fn, const size_t np); //!< save with Cini loaded

            //! f0 at Cini, f1 at Cend => f_opt at Ctmp
            xReal                    optimize(const xReal f0, const xReal f1);
            
            const Mix   &      mix;          //!< persistent mix
            Engine             my;
            XMatrix            Csolve;       //!< all possible solving C
            XMatrix            deltaC;       //!< all possible delta   C
            XArray             Cini;         //!< starting point
            XArray             Cend;         //!< final point
            XArray             Ctmp;         //!< updated by objectiveFunction(u)
            XArray             step;         //!< temporary computed step
            XArray             Cerr;         //!< error on computed C
            XAdds              Cadd;         //!< for algebra on rates
            Orthogonal::Family ortho;
            ProList            basis;        //!< basis of leading prospects
            MKL::LU<xReal>     lu;           //!< to solve algebraic systems
            XArrays            xiArr;        //!< arrays of xi
            XMatrices          PhiMat;       //!< arrays of Phi
            XMatrices          NuMat;        //!< arrays of Nu
            XMatrices          NuTMat;       //!< arrays of Nu'
            XMatrices          XiMat;        //!< arrays of Xi
            Cursors            crs;          //!< cursors

        public:
            const xReal        one;  //!< 1.0
            const xReal        safe; //!< 0.99

        };
    }

}

#endif

