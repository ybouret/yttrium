
#include "y/woven/subspaces.hpp"
#include "y/woven/integer-survey.hpp"
#include "y/woven/natural-survey.hpp"

#include "y/utest/run.hpp"
#include "../main.hpp"


using namespace Yttrium;

namespace
{



}

Y_UTEST(woven_subspace)
{
    Random::Rand   ran;
    Matrix<int>    mu(4,3);

    for(size_t i=1;i<=mu.rows;++i)
    {
        for(size_t j=1;j<=mu.cols;++j)
        {
            while( 0 == (mu[i][j] = ran.in<int>(-2,2)) )
                ;
        }
    }

    std::cerr << "mu=" << mu << std::endl;

    {
        WOVEn::IntegerSurvey survey;
        {
            WOVEn::Subspaces spaces(mu,NULL);

            std::cerr << spaces << std::endl;

            while(spaces.generate(mu, &survey))
            {
            }
            survey.sort();

            for(const WOVEn::IntegerArray *arr=survey.head;arr;arr=arr->next)
            {
                std::cerr << " (+) " << *arr << std::endl;
            }
        }
    }

    {
        WOVEn::NaturalSurvey survey;
        {
            WOVEn::Subspaces spaces(mu,&survey);

            std::cerr << spaces << std::endl;

            while(spaces.generate(mu, &survey))
            {
            }
            survey.sort();

            for(const WOVEn::NaturalArray *arr=survey.head;arr;arr=arr->next)
            {
                std::cerr << " (*) " << *arr << std::endl;
            }
        }

    }

    Y_SIZEOF(WOVEn::QVector);
    

}
Y_UDONE()

