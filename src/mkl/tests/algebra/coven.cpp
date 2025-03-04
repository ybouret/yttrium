#include "y/apex/api/ortho/coven/survey/natural.hpp"
#include "y/apex/api/ortho/coven/survey/integer.hpp"


#include "y/utest/run.hpp"
#include "y/string/env.hpp"
#include "y/mkl/algebra/ortho-space.hpp"
#include "y/apex/api/count-non-zero.hpp"

using namespace Yttrium;
using namespace Apex;

namespace Yttrium
{
    namespace Apex
    {
        namespace Ortho
        {
            namespace Coven
            {

               

               



            }
        }
    }


}

template <typename MATRIX> static inline
void DoProcess(XMLog &       xml,
               const MATRIX &M)
{
    Y_XML_SECTION(xml, "Processing");
    std::cerr << "M=" << M << std::endl;
    Ortho::Coven::Callback proc = cfunctor2( Ortho::Coven::Tribes::Display );
    {
        Y_XML_SECTION(xml, "Combinations");
        const MATRIX data(TransposeOf,M);
        Ortho::Coven::IntegerSurvey survey(xml,data,0);
        std::cerr << "IntegerSurvey=" << survey << std::endl;
    }


    {
        Y_XML_SECTION(xml, "Conservations");
        Matrix<apz> Q;
        if( ! MKL::OrthoSpace::Make(Q,M) )
            throw Exception("No OrthoSpace!!");
        std::cerr << "Q=" << Q << std::endl;
        Ortho::Coven::NaturalSurvey survey(xml,Q,0);
        std::cerr << "NaturalSurvey=" << survey << std::endl;

    }

    std::cerr << std::endl;

}

Y_UTEST(algebra_coven)
{
    bool  verbose = Environment::Flag("VERBOSE");
    XMLog xml(verbose);

    // one acid
    {
        Matrix<int> M(2,4);
        /* H20 <=> H+ + OH- */ M[1][1] = 1; M[1][2] = 1;
        /* AH  <=> H+ + A-  */ M[2][1] = 1; M[2][3] = -1; M[2][4] = 1;
        DoProcess(xml,M);
    }


    //! two acids
    {
        Matrix<int> M(3,6);
        /* H20 <=> H+ + OH-  */ M[1][1] = 1; M[1][2] = 1;
        /* AH  <=> H+ + A-   */ M[2][1] = 1; M[2][3] = -1; M[2][4] = 1;
        /* NH4 <=> H+ + NH3- */ M[3][1] = 1; M[3][5] = -1; M[3][6] = 1;
        DoProcess(xml,M);
    }

    //! diacid
    {
        Matrix<int> M(3,5);
        /* H20 <=> H+ + OH-  */ M[1][1] = 1; M[1][2] = 1;
        /* AH2 <=> H+ + AH-  */ M[2][1] = 1; M[2][3] = -1; M[2][4] = 1;
        /* AH- <=> H+ + A2-  */ M[3][1] = 1; M[3][3] = -1; M[3][5] = 1;
        DoProcess(xml,M);
    }


    Y_SIZEOF( Ortho::Coven::SArray<apz> );
    Y_SIZEOF( Ortho::Coven::SArray<apn> );


}
Y_UDONE()


