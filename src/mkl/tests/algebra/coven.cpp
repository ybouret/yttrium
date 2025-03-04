#include "y/apex/api/ortho/coven/tribes.hpp"
#include "y/utest/run.hpp"
#include "y/string/env.hpp"
#include "y/mkl/algebra/ortho-space.hpp"

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


                template <typename T>
                class SArray : public Quantized, public CxxArray<T,Memory::Dyadic>
                {
                public:
                    typedef CxxArray<T,Memory::Dyadic> SArrayType;

                    explicit SArray(const Vector &v) :
                    SArrayType(CopyOf,v),
                    next(0),
                    prev(0)
                    {
                    }

                    virtual ~SArray() noexcept
                    {
                    }

                    SArray *next;
                    SArray *prev;



                private:
                    Y_DISABLE_COPY_AND_ASSIGN(SArray);
                };

                class Survey
                {
                protected:
                    virtual ~Survey() noexcept
                    {
                    }

                public:
                    explicit Survey() :
                    proc(this, & Survey::check)
                    {
                    }


                    virtual void carryOut(const IList &l, const Vector &v) = 0;



                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Survey);
                    Callback proc;

                    void check(const IList &l, const Vector &v)
                    {
                        carryOut(l,v);
                    }
                };

                template <typename T>
                class SurveyOf : public Survey, public Proxy< const ListOf< SArray<T> > >
                {
                public:
                    typedef SArray<T> ArrayType;
                    typedef Proxy< ListOf<ArrayType> > ProxyType;

                    virtual ~SurveyOf() noexcept
                    {
                    }

                protected:
                    explicit SurveyOf() noexcept : my()
                    {
                    }


                    CxxListOf<ArrayType> my;

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(SurveyOf);
                    inline virtual typename ProxyType::ConstInterface & surrogate() const noexcept
                    {
                        return my;
                    }

                };

                class IntegerSurvey : public SurveyOf<Integer>
                {
                public:
                    explicit IntegerSurvey() noexcept {}
                    virtual ~IntegerSurvey() noexcept {}

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(IntegerSurvey);
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
    Ortho::Coven::Callback proc = cfunctor2( Ortho::Coven::Tribes::Display );
    {
        Y_XML_SECTION(xml, "Combinations");
        const MATRIX data(TransposeOf,M);
        Ortho::Coven::Wayfarer::Walk(xml,proc,data);
    }

    {
        Y_XML_SECTION(xml, "Conservations");
        Matrix<apz> Q;
        if( ! MKL::OrthoSpace::Make(Q,M) )
            throw Exception("No OrthoSpace!!");
        Y_XMLOG(xml,"Q=" << Q);
        Ortho::Coven::Wayfarer::Walk(xml,proc,Q);
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


    Y_SIZEOF( Ortho::Coven::SArray<apz> );
    Y_SIZEOF( Ortho::Coven::SArray<apn> );


}
Y_UDONE()


