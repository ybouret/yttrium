
//! \file

#ifndef Y_Chemical_Solver_Included
#define Y_Chemical_Solver_Included 1

#include "y/chemical/plexus/mix.hpp"
#include "y/chemical/plexus/solver/engine.hpp"
#include "y/chemical/reactive/equilibrium/aftermath.hpp"
#include "y/orthogonal/family.hpp"

namespace Yttrium
{
    namespace Chemical
    {

     
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
            static const char * const CallSign; //!< "Chemical::Solver"
            static const char * const ODE;      //!< "ode"
            static const char * const NRA;      //!< "nra"

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Solver(const Mix &); //!< setup from persistent mix
            virtual ~Solver() noexcept;   //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void run(XMLog &xml, XWritable &C, const Level L, const XReadable &K);

            xReal operator()(const xReal u);


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Solver);
            virtual ConstInterface & surrogate() const noexcept; //!< [Proxy]

            xReal                    objectiveFunction(const XReadable &C, const Level L);
            xReal                    objectiveFunction(const xReal u);

            xReal                    buildODE(XMLog &xml, const xReal f0);
            xReal                    buildNRA(XMLog &xml, const xReal f0);
            xReal                    makeStep(XMLog &xml, const xReal f0, const char * const sid);

            void                     save(OutputStream &fp, const size_t np);    //!< save with Cini/Cend loaded
            void                     save(const String &fn, const size_t np);    //!< save with Cini/Cend loaded
            void                     savePro(const String &fn, const size_t np); //!< save with Cini loaded

            //! f0 at Cini, f1 at Cend => f_opt at Ctmp
            xReal                    optimize(const xReal f0, const xReal f1);
            
            const Mix   &      mix;          //!< persistent mix
            Engine             my;
            XMatrix            Csolve;       //!< all possible solving C
            XMatrix            deltaC;       //!< all possible delta   C
            Orthogonal::Family family;
            XArray             Cini;         //!< starting point
            XArray             Cend;         //!< final point
            XArray             Ctmp;         //!< updated by objectiveFunction(u)
            XArray             step;         //!< temporary computed step
            XAdds              Cadd;         //!< for algebra on rates
            const xReal        one;

        };
    }

}

#endif
