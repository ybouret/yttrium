//! \file

#ifndef Y_Chemical_Device_Included
#define Y_Chemical_Device_Included 1

#include "y/chemical/plexus/device/ansatz.hpp"
#include "y/chemical/plexus/joint.hpp"
#include "y/chemical/reactive/aftermath.hpp"
#include "y/mkl/algebra/lu.hpp"
#include "y/orthogonal/family.hpp"
#include "y/chemical/plexus/outcome.hpp"
#include "y/data/small/light/list/solo.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        // Helper for Ansatz
        //
        //______________________________________________________________________
        typedef Small::SoloLightList<const Ansatz> AList; //!< alias for basis
        typedef AList::NodeType                    ANode; //!< alias

        //______________________________________________________________________
        //
        //
        //
        //! Device to solve phase space
        //
        //
        //______________________________________________________________________
        class Device : public Joint
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Device(const Cluster &, const bool doTrace); //!< setup for cluster
            virtual ~Device() noexcept;                           //!< cleanup


            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! process up to convergence
            void    operator()(XWritable       &C,
                               const Level      L,
                               const XReadable &K,
                               XMLog           &xml);

            //! best guess one-step of processing
            /**
             \param C input/output phase space
             \param L level of C
             \param K TopLevel constants
             \param xml for verbosity
             */
            Outcome process(XWritable       &C,
                            const Level      L,
                            const XReadable &K,
                            XMLog           &xml);

            //! objectiveFunction at Ctmp  = Cini(1-u)+Cend*u u in [0:1]
            /**
             \param   u in [0:1]
             \return  objectiveFunction( Cini(1-u)+Cend*u )
             \warning Cini, Cend and ansatz must be defined!!
             */
            xreal_t operator()(const xreal_t u);


            //! check all basis eqs are ok with given concentrations
            bool basisOkWith(const XReadable &C, const Level L) const noexcept;

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


            //! saving profile between Cini and Cend. Warning, changes Ctmp
            void saveProfile(const String &fileName, const size_t np);

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            Aftermath          aftermath; //!< helper to solve each equilibrium
            XMatrix            EqConc;    //!< store concentrations
            XMatrix            EqDiff;    //!< store delta
            Ansatz::Series     ansatz;    //!< (lightweight) ansatz
            XArray             Cini;      //!< Cini to start searching
            XArray             Cend;      //!< Cend to finish search
            XArray             Ctmp;      //!< temporary phase space
            XArray             Copt;      //!< optimal phase space
            XArray             dC;        //!< temporary dC
            xreal_t            ff0;       //!< objective function at Cini
            xreal_t            ff1;       //!< objective function at Cend
            XSeries            objValue;  //!< to compute objective function
            XArray             gradient;  //!< gradient at Cini
            XSwell             increase;  //!< to help compute gradient
            const size_t       dof;       //!< initial equilibria
            Orthogonal::Family ortho;     //!< helper to find basis
            AList              basis;     //!< basis of independent leading equilibria
            const xreal_t      shield;    //!< 0.99
            XMatrices          xPhi;      //!< Phi[1..dof,nspc]
            XMatrices          xNu;       //!< Nu[1..dof,nspc]
            XMatrices          xChi;      //!< Chi[1..dof,1..dof]
            XArrays            xXi;       //!< xi[1..dof]
            MKL::LU<xreal_t>   xlu;       //!< LU solver
            bool               trace;     //!< trace

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Device);
            void  computeRate(XWritable &rate); //!< local rates from extents
            void  showAnsatz(XMLog &xml) const; //!< helper

            //! objective Function for defined ansatz
            xreal_t objectiveFunction(const XReadable &C, const Level L);

            //! objective gradient for defined ansatz
            xreal_t objectiveGradient(const XReadable &C, const Level L);


            bool    enhance(Ansatz &);           //!< enhance or nullify ansatz
            bool    nullify(Ansatz &) noexcept;  //!< nullify ansatz and return false

            //! minimize between F(0) = objectiveFunction(Cini) and F(1) = objectiveFunction(Cend)
            /**
             \return optimized F, Ctmp is computed
             */
            xreal_t lookUp();


        };
    }

}

#endif
