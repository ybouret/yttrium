
//! \file

#ifndef Y_Chemical_Solver_Included
#define Y_Chemical_Solver_Included 1

#include "y/chemical/plexus/mix.hpp"
#include "y/chemical/plexus/solver/prospect.hpp"
#include "y/chemical/reactive/equilibrium/aftermath.hpp"

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
        class Solver : public Aftermath, public Assembly
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const CallSign; //!< "Chemical::Solver"

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
            void show(XMLog &xml) const;

            void update() noexcept; //!< update assembly from prospect

            xReal objectiveFunction(const XReadable &C, const Level L);
            xReal objectiveFunction(const xReal u);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Solver);

            const Mix   & mix;          //!< persistent mix
            XMatrix       Csolve;       //!< all possible solving C
            XMatrix       deltaC;       //!< all possible delta   C
            XArray        Cini;         //!< starting point
            XArray        Cend;         //!< final point
            XArray        Ctmp;         //!< updated by objectiveFunction(u)
            ProBank       pbank;        //!< resources
            ProList       plist;        //!< list of active prospects

        };
    }

}

#endif

