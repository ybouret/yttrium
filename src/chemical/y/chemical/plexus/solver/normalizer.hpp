
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
        class Normalizer : public Quantized, public Counted
        {
        public:
            static const char * const   CallSign;
            typedef LittleEndianKey     KeyType;
            typedef LittleEndianAddress KeyType_;

            typedef ArkPtr<KeyType,Normalizer> Ptr;
            typedef SuffixSet<KeyType,Ptr>     Set;

            explicit Normalizer(const Cluster &cl);

            virtual ~Normalizer() noexcept;

            const KeyType & key() const noexcept;

            void run(XWritable       & Ctop,
                     const XReadable & Ktop,
                     XMLog           & xml);

            xreal_t  operator()(const xreal_t u);

            const Cluster &    rcl; //!< reference to cluster
            const KeyType_     lek; //!< key
            const size_t       nsp; //!< number of species
            Aftermath          afm; //!< aftermath
            XMatrix            ceq; //!< storage of phase space
            Applicant::Series  aps; //!< applicant store
            CxxSeries<xreal_t> obj; //!< for objective function
            CxxArray<xreal_t>  Cin; //!< input  phase space
            CxxArray<xreal_t>  Cex; //!< output  phase space
            CxxArray<xreal_t>  Cws; //!< working phase space
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

            //! after a successful compilation
            xreal_t overall(XWritable       & Ctop,
                            const bool        repl,
                            XMLog           & xml);

            bool NDSolve(XWritable       &Ctop,
                         const XReadable &Ktop,
                         XMLog           &xml);

            //! improve by simplex lookup
            xreal_t  improve(XWritable       & Ctop,
                             const bool        repl,
                             XMLog           & xml);

            static SignType CompareBasis(const ANode * const lhs, const ANode * const rhs) noexcept;
            
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

