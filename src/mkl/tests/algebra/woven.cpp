

#include "y/woven/subspaces.hpp"
#include "y/woven/survey/integer.hpp"
#include "y/utest/run.hpp"
#include "y/text/ascii/convert.hpp"
#include "y/sequence/vector.hpp"
#include "y/mkl/algebra/ortho-space.hpp"
using namespace Yttrium;



Y_UTEST(algebra_woven)
{

    Vector<unsigned> coef;
    for(int i=1;i<argc;++i)
    {
        coef << ASCII::Convert::To<unsigned>(argv[i],"coef");
    }

    if(coef.size()<2)
    {
        std::cerr << "not enough coefficients" << std::endl;
        return 0;
    }

    const size_t dims = coef.size();
    Matrix<unsigned> P(1,dims);
    for(size_t i=dims;i>0;--i) P[1][i] = coef[i];
    Matrix<apz> Q;
    if(!MKL::OrthoSpace::Make(Q,P))
    {
        throw Exception("couldn't make ortho space!");
    }
    std::cerr << "Q=" << Q << std::endl;
    bool  verbose = true;
    XMLog xml(verbose);
    WOVEn::IntegerSurvey survey(xml);
    WOVEn::Explore(Q,survey,true);

    size_t indx=0;
    for(const WOVEn::IntegerArray *arr=survey.head;arr;arr=arr->next)
    {
        std::cerr << "Q" << ++indx << " = " << *arr << std::endl;
    }

}
Y_UDONE()
