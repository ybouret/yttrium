
#include "y/woven/subspaces.hpp"
#include "y/utest/run.hpp"
#include "../main.hpp"


using namespace Yttrium;

namespace
{

    struct DoSomething
    {

        void With(const WOVEn::QVector &q)
        {
            std::cerr << " (*) " << q << std::endl;
        }
    };

}

Y_UTEST(woven_subspace)
{
    Random::Rand  ran;
    Matrix<int>   mu(4,5);

    DoSomething    dum;
    WOVEn::QSurvey survey(&dum, &DoSomething::With);

    for(size_t i=1;i<=mu.rows;++i)
    {
        for(size_t j=1;j<=mu.cols;++j)
        {
            while( 0 == (mu[i][j] = ran.in<int>(-2,2)) )
                ;
        }
    }

    std::cerr << "mu=" << mu << std::endl;

    WOVEn::Subspaces spaces(mu,&survey);

    std::cerr << spaces << std::endl;

    while(spaces.generate(mu, &survey))
    {
    }


}
Y_UDONE()

