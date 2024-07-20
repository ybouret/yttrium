
#include "y/woven/subspaces.hpp"
#include "y/woven/survey/integer.hpp"
#include "y/woven/survey/natural.hpp"

#include "y/utest/run.hpp"
#include "../main.hpp"
#include "y/text/ascii/convert.hpp"

using namespace Yttrium;



Y_UTEST(woven_subspace)
{
    Random::Rand   ran;

    size_t         nr = 4;
    size_t         nc = 3;
    if(argc>1)
    {
        nr = ASCII::Convert::To<size_t>(argv[1],"rows");
    }
    
    if(argc>2)
    {
        nc = ASCII::Convert::To<size_t>(argv[2],"cols");
    }

    Matrix<int>    mu(nr,nc);

    for(size_t i=1;i<=mu.rows;++i)
    {
        for(size_t j=1;j<=mu.cols;++j)
        {
            while( 0 == (mu[i][j] = ran.in<int>(-2,2)) )
                ;
        }
    }

    std::cerr << "mu=" << mu << std::endl;
    bool  verbose = true;
    XMLog xml(verbose);
    {
        WOVEn::IntegerSurvey survey(xml);
        Y_XML_SECTION(xml, "IntegerSurvey");
        WOVEn::Explore(mu,survey,false);

    }

    if(true)
    {
        WOVEn::NaturalSurvey survey(xml);
        Y_XML_SECTION(xml, "NaturalSurvey");
        WOVEn::Explore(mu,survey,true);
    }

    Y_SIZEOF(WOVEn::QVector);
    Y_SIZEOF(WOVEn::SubSpace);


}
Y_UDONE()

