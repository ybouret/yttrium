
#ifndef Y_Chemical_Normalizer_Included
#define Y_Chemical_Normalizer_Included 1

#include "y/chemical/plexus/solver/applicant.hpp"
#include "y/chemical/plexus/solver/simplex.hpp"
#include "y/chemical/reactive/aftermath.hpp"
#include "y/chemical/plexus/cluster.hpp"
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
        //! Normalizer for a persistent Cluster
        //
        //
        //______________________________________________________________________
        class Normalizer : public Quantized, public Counted
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const          CallSign; //!< uuid
            typedef LittleEndianKey            KeyType;  //!< alias
            typedef LittleEndianAddress        KeyType_; //!< alias
            typedef ArkPtr<KeyType,Normalizer> Ptr;      //!< alias
            typedef SuffixSet<KeyType,Ptr>     Set;      //!< alias

            //! NDSolve result
            enum Result
            {
                Success, //!< better than fortified
                Failure, //!< singular concentrations
                Partial  //!< worst than fortified
            };

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Normalizer(const Cluster &cl); //!< setup for cluster
            virtual ~Normalizer() noexcept;         //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            const KeyType & key() const noexcept; //!< internal key

            //! algo...
            bool run(XWritable       & Ctop,
                     const XReadable & Ktop,
                     XMLog           & xml);

            //! objective function of (1-u)*Cin + u * Cex
            xreal_t  operator()(const xreal_t u);


            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________

            const Cluster &    rcl; //!< reference to cluster
            const KeyType_     lek; //!< key
            const size_t       nsp; //!< number of species
            Aftermath          afm; //!< aftermath
            XMatrix            ceq; //!< storage of phase space
            Applicant::Series  aps; //!< applicant store
            CxxSeries<xreal_t> obj; //!< for objective function
            XArray             Cin; //!< input  phase space
            XArray             Cex; //!< output  phase space
            XArray             Cws; //!< working phase space
            XArray             Cst; //!< starting phase space
            XArray             dCs; //!< delta C
            const size_t       dof; //!< primary eqs
            ABank              bnk; //!< pool of applicants
            AList              apl; //!< applicant list
            Orthogonal::Family qfm; //!< orthogonal family
            Simplex            sim; //!< simplex
            const XMatrix      pro; //!< projection matrix
            const xreal_t      psf; //!< projection scaling factor
            CxxSeries<XMatrix> Phi; //!< matrices phi
            CxxSeries<XMatrix> XNu; //!< matrices Nu
            CxxSeries<XMatrix> Chi; //!< matrices chi
            CxxSeries<XArray>  Lhs; //!< arrays
            MKL::LU<xreal_t>   xlu; //!< linear algebra

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Normalizer);

            xreal_t objectiveFunction(const XReadable &C, const Level L);

            //! assuming C0 is ok, fine tune C1
            void project(XWritable        &C1,
                         const Level       L1,
                         const XReadable  &C0,
                         const Level       L0);

            //! build projection matrix: Nu'*inv(Nu*Nu')*Nu as pro/psf
            void setProj();


            //! compile and improve
            bool fortify(XWritable       & Ctop,
                         const XReadable & Ktop,
                         XMLog           & xml);

            //! one ND step
            Result NDSolve(XWritable       &Ctop,
                           const XReadable &Ktop,
                           XMLog           &xml);




            //! extract a basis of equilibria
            size_t   extract(XMLog &xml);

            //! compile all possible applicant
            size_t compile(XWritable       & Ctop,
                           const XReadable & Ktop,
                           bool            & repl,
                           XMLog           &  xml);

        };
    }

}

#endif

