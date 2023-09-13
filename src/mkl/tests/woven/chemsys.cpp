

#include "y/woven/subspaces.hpp"
#include "y/woven/integer-survey.hpp"
#include "y/woven/natural-survey.hpp"
#include "y/apex/mylar.hpp"
#include "y/utest/run.hpp"
#include "y/mkl/algebra/rank.hpp"
#include "y/mkl/algebra/ortho-space.hpp"

using namespace Yttrium;

// H+ HO- AH Am NH4+ NH3 OxH2 OxH- Ox--

#define h    1
#define w    2
#define AH   3
#define Am   4
#define NH4  5
#define NH3  6
#define OxH2 7
#define OxH  8
#define Ox   9

Y_UTEST(woven_chemsys)
{
    //Random::Rand   ran;
    const size_t M = 9;
    const size_t N = 5;



    Matrix<int> Nu(N,M);

    // water
    {
        Writable<int> &nu = Nu[1];
        nu[h] = 1;
        nu[w] = 1;
    }

    // AH <=> Am + H+
    {
        Writable<int> &nu = Nu[2];
        nu[h]  =  1;
        nu[Am] =  1;
        nu[AH] = -1;
    }

    // NH4+ <=> NH3 + H+
    {
        Writable<int> &nu = Nu[3];
        nu[h]   =  1;
        nu[NH3] =  1;
        nu[NH4] = -1;
    }

    // OxH2 <=> OxH- + H+
    {
        Writable<int> &nu = Nu[4];
        nu[h]    =  1;
        nu[OxH]  =  1;
        nu[OxH2] = -1;
    }

    // OxH- <=> Ox-- + H+
    {
        Writable<int> &nu = Nu[5];
        nu[h]    =  1;
        nu[Ox]   =  1;
        nu[OxH]  = -1;
    }


    std::cerr << "Nu=" << Nu << std::endl;
    std::cerr << "#"; Y_CHECK( MKL::Rank::Of(Nu) == N);

    // compute ortho-space
    Matrix<apz> Q;
    MKL::OrthoSpace::Build(Q,CopyOf,Nu);
    std::cerr << "Q=" << Q << std::endl;

    // compute conservations
    WOVEn::NaturalSurvey      zcons;
    WOVEn::Subspaces::Explore(Q,zcons,true);
    zcons.sort();
    
    for(const WOVEn::NaturalArray *zcf=zcons.head;zcf;zcf=zcf->next)
    {
        std::cerr << *zcf << std::endl;
    }
}
Y_UDONE()

