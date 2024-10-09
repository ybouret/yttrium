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

        typedef Small::SoloLightList<const Ansatz> AList;
        typedef AList::NodeType                    ANode;

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
            explicit Device(const Cluster &); //!< setup for cluster
            virtual ~Device() noexcept;       //!< cleanup


            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            Outcome process(XWritable       &C,
                            const Level      L,
                            const XReadable &K,
                            XMLog           &xml);

            void showAnsatz(XMLog &xml) const;

            xreal_t objectiveFunction(const XReadable &C, const Level L);
            xreal_t objectiveGradient(const XReadable &C, const Level L);
            xreal_t operator()(const xreal_t u); //!< objFunc @Ctmp  = Cini(1-u)+Cend*u u in [0:1]
            bool    enhance(Ansatz &);           //!< enhance/nullify ansatz
            bool    nullify(Ansatz &) noexcept;  //!< nullify and return false

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
            XSeries            objValue;  //!< to compute objective function
            XArray             gradient;  //!< gradient at Cini
            XSwell             increase;  //!< to help compute gradient
            const size_t       dof;       //!< initial equilibria
            Orthogonal::Family ortho;     //!< helper to find basis
            AList              basis;
            const xreal_t      shield;    //!< 0.99
            XMatrices          xPhi;      //!< Phi[1..dof,nspc]
            XMatrices          xNu;       //!< Nu[1..dof,nspc]
            XMatrices          xChi;      //!< Chi[1..dof,1..dof]
            XArrays            xXi;       //!< xi[1..dof]
            MKL::LU<xreal_t>   xlu;       //!< LU solver
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Device);
            void  computeRate(XWritable &rate);

        };
    }

}

#endif
