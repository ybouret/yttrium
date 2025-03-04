#include "y/apex/api/ortho/coven/tribes.hpp"
#include "y/utest/run.hpp"
#include "y/container/matrix.hpp"
#include "y/string/env.hpp"

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

                struct Wayfarer
                {
                    template <typename MATRIX> static inline
                    void Walk(XMLog        &   xml,
                              Callback     &   proc,
                              const MATRIX &   data,
                              uint64_t * const ell = 0)
                    {
                        const Metrics params(data.cols);
                        Ortho::VCache vCache( new Ortho::Vector::Cache(params) );
                        Ortho::FCache fCache( new Ortho::Family::Cache(vCache) );
                        IBank         iCache;

                        Ortho::Coven::Tribes   tribes(xml,proc,data,iCache,fCache,ell);
                        while(tribes.size)
                        {
                            tribes.generate(xml,proc,data,Strategy::Optimize,ell);
                        }
                    }


                };


            }
        }
    }


}

template <typename MATRIX> static inline
void DoProcess(XMLog &xml,
               const MATRIX &M)
{
    Y_XML_SECTION(xml, "Processing");
    Y_XMLOG(xml,"M=" << M);
    {
        const MATRIX data(TransposeOf,M);
        Ortho::Coven::Callback proc = cfunctor( Ortho::Coven::Tribes::Display );
        Ortho::Coven::Wayfarer::Walk(xml,proc,data);
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


