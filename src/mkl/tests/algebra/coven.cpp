#include "y/apex/api/ortho/coven/tribes.hpp"
#include "y/utest/run.hpp"
#include "y/container/matrix.hpp"
#include "y/string/env.hpp"

using namespace Yttrium;
using namespace Apex;

namespace Yttrium
{
    namespace MKL
    {
        
    }
}

template <typename MATRIX> static inline
void DoProcess(XMLog &xml,
               const MATRIX &M)
{
    Y_XML_SECTION(xml, "Processing");
    Y_XMLOG(xml,"M=" << M);
    const MATRIX data(TransposeOf,M);

    Ortho::Coven::IBank    bank;
    Ortho::Metrics         qmtx(data.cols);
    Ortho::VCache          qvcc( new Ortho::Vector::Cache(qmtx) );
    Ortho::FCache          qfcc( new Ortho::Family::Cache(qvcc) );
    Ortho::Coven::Callback proc = cfunctor( Ortho::Coven::Tribes::Display );
    Ortho::Coven::Tribes   tribes(xml,proc,data,bank,qfcc,0);
    while(tribes.size)
    {
        tribes.generate(xml,proc,data,0,0);
    }
}

Y_UTEST(algebra_coven)
{
    bool  verbose = Environment::Flag("VERBOSE");
    XMLog xml(verbose);

    {
        Matrix<int> M(2,4);
        /* H20 <=> H+ + OH- */ M[1][1] = 1; M[1][2] = 1;
        /* AH  <=> H+ + A-  */ M[2][1] = 1; M[2][3] = -1; M[2][4] = 1;
        DoProcess(xml,M);
    }




}
Y_UDONE()


