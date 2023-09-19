
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
        {
            WOVEn::Subspaces spaces(mu,0,xml);

            std::cerr << spaces << std::endl;
            {
                Y_XML_SECTION(xml, "Generating");
                while(spaces.generate(mu, &survey))
                {
                }
                survey.sort();
            }

            for(const WOVEn::IntegerArray *arr=survey.head;arr;arr=arr->next)
            {
                Y_XMLOG(xml," (+) " << *arr);
            }
        }
    }

    {
        WOVEn::NaturalSurvey survey(xml);
        Y_XML_SECTION(xml, "NaturalSurvey");
        {
            WOVEn::Subspaces spaces(mu,&survey,xml);

            std::cerr << spaces << std::endl;
            {
                Y_XML_SECTION(xml, "Generating");
                while(spaces.generate(mu, &survey))
                {
                }
                survey.sort();
            }

            for(const WOVEn::NaturalArray *arr=survey.head;arr;arr=arr->next)
            {
                Y_XMLOG(xml," (+) " << *arr);
            }
        }

    }

    Y_SIZEOF(WOVEn::QVector);
    

}
Y_UDONE()

