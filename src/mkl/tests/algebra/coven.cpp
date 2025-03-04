#include "y/apex/api/ortho/coven/tribes.hpp"
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

                struct Wayfarer
                {
                    template <typename MATRIX> static inline
                    void Walk(XMLog        &   xml,
                              Callback     &   proc,
                              const MATRIX &   data,
                              uint64_t * const ell)
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
                    ncof(CountNonZero(*this)),
                    next(0),
                    prev(0)
                    {
                    }

                    virtual ~SArray() noexcept
                    {
                    }

                    const size_t ncof;
                    SArray *     next;
                    SArray *     prev;



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


                    virtual void study(const IList &l, const Vector &v) = 0;



                protected:
                    Callback proc;

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Survey);

                    void check(const IList &l, const Vector &v)
                    {
                        study(l,v);
                    }
                };

                template <typename T>
                class SurveyOf : public Survey, public Proxy< const ListOf< SArray<T> > >
                {
                public:
                    typedef SArray<T>                  ArrayType;
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

                    template <typename MATRIX> inline
                    explicit IntegerSurvey(XMLog        &   xml,
                                           const MATRIX &   data,
                                           uint64_t * const pEll)
                    {
                        Wayfarer::Walk(xml, proc, data, pEll);
                    }

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(IntegerSurvey);

                    virtual void study(const IList &l, const Vector &v)
                    {
                        if(l.size>1 && CountNonZero(v) >= 2)
                        {
                            my.pushTail(new ArrayType(v) );
                            std::cerr << "\t(comb) " << *my.tail << std::endl;
                        }
                    }
                };

                class NaturalSurvey : public SurveyOf<Natural>
                {
                public:
                    explicit NaturalSurvey() noexcept {}
                    virtual ~NaturalSurvey() noexcept {}

                    template <typename MATRIX> inline
                    explicit NaturalSurvey(XMLog        &   xml,
                                           const MATRIX &   data,
                                           uint64_t * const pEll)
                    {
                        Wayfarer::Walk(xml, proc, data, pEll);
                    }

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(NaturalSurvey);

                    virtual void study(const IList &, const Vector &v)
                    {
                        size_t numPositive = 0;
                        for(size_t i=v.size();i>0;--i)
                        {
                            switch( v[i].s )
                            {
                                case Negative: return;
                                case __Zero__: continue;
                                case Positive:
                                    ++numPositive;
                                    continue;
                            }
                        }
                        if(numPositive>=2)
                        {
                            my.pushTail(new ArrayType(v) );
                            std::cerr << "\t(keep) " << *my.tail << std::endl;
                        }


                    }
                };




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
    }


    {
        Y_XML_SECTION(xml, "Conservations");
        Matrix<apz> Q;
        if( ! MKL::OrthoSpace::Make(Q,M) )
            throw Exception("No OrthoSpace!!");
        std::cerr << "Q=" << Q << std::endl;
        Ortho::Coven::NaturalSurvey survey(xml,Q,0);
    }

    std::cerr << std::endl;

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

    
    {
        Matrix<int> M(3,6);
        /* H20 <=> H+ + OH-  */ M[1][1] = 1; M[1][2] = 1;
        /* AH  <=> H+ + A-   */ M[2][1] = 1; M[2][3] = -1; M[2][4] = 1;
        /* NH4 <=> H+ + NH3- */ M[3][1] = 1; M[3][5] = -1; M[3][6] = 1;
        DoProcess(xml,M);
    }


    Y_SIZEOF( Ortho::Coven::SArray<apz> );
    Y_SIZEOF( Ortho::Coven::SArray<apn> );


}
Y_UDONE()


