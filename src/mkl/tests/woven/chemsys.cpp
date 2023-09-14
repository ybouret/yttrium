

#include "y/woven/subspaces.hpp"
#include "y/woven/survey/integer.hpp"
#include "y/woven/survey/natural.hpp"
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

namespace
{
    class Topo : public Object, public CxxArray<int,Memory::Pooled>
    {
    public:
        typedef CxxArray<int,Memory::Pooled> TopoArray;
        typedef CxxListOf<Topo>              List;

        explicit Topo(const Readable<apz> &nu) :
        TopoArray(nu.size()),
        next(0),
        prev(0)
        {
            for(size_t i=size();i>0;--i)
            {
                (*this)[i] = nu[i].cast<int>(0);
            }
        }

        virtual ~Topo() noexcept
        {

        }

        Topo * next;
        Topo * prev;


    private:
        Y_DISABLE_COPY_AND_ASSIGN(Topo);
    };



}



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

    // conservation laws
    WOVEn::NaturalSurvey  conserv;
    {
        // compute ortho-space
        Matrix<apz> NuOrtho;
        MKL::OrthoSpace::Make(NuOrtho,Nu);
        std::cerr << "NuOrtho=" << NuOrtho << std::endl;

        // compute conservations
        WOVEn::Explore(NuOrtho,conserv,true);
        conserv.sort();
    }

    // transfer to matrix
    Matrix<unsigned> Q;
    if(conserv.size)
    {
        Q.make(conserv.size,M);
        size_t i=1;
        for(const WOVEn::NaturalArray *zcf=conserv.head;zcf;zcf=zcf->next,++i)
        {
            for(size_t j=M;j>0;--j) Q[i][j] = (*zcf)[j].cast<unsigned>("q");
        }
    }
    std::cerr << "Q=" << Q << std::endl;

    // combinations
    WOVEn::IntegerSurvey zeroing;
    const Matrix<int> NuT(TransposeOf,Nu);
    std::cerr << "NuT=" << NuT << std::endl;
    Matrix<int> NuTx;
    Apex::Mylar::Compress(NuTx,NuT);
    std::cerr << "NuTx=" << NuTx << std::endl;


    WOVEn::Explore(NuTx,zeroing,false);

    for(const WOVEn::IntegerArray *arr=zeroing.head;arr;arr=arr->next)
    {
        std::cerr << *arr << std::endl;
    }
    std::cerr << "#=" << zeroing.size << std::endl;

    std::cerr << "Nu=" << Nu << std::endl;

    // creating new reaction
    Topo::List topoList;

    CxxArray<apz,Memory::Pooled> coeff(M);
    Matrix<apz>                  apNuT(CopyOf,NuT);
    WOVEn::Indices               incoming(M);
    WOVEn::Indices               outgoing(M);

    for(const WOVEn::IntegerArray *arr=zeroing.head;arr;arr=arr->next)
    {
        const Readable<const apz> &weight = *arr;
        apNuT.mul(coeff,weight);
        std::cerr << "coeff: " << coeff << std::endl;
        AutoPtr<Topo> topo = new Topo(coeff);

        // look for duplicate
        {
            bool          isOk = true;
            for(const Topo *node=topoList.head;node;node=node->next)
            {
                if(*topo==*node)
                {
                    isOk = false;
                    break;
                }
            }

            if(!isOk )
            {
                // shouldn't happen ?
                std::cerr << "*** duplicate" << std::endl;
                continue;
            }
        }

        // look for meaningful combination
        incoming.free();
        for(size_t i=N;i>0;--i)
        {
            if( 0 == weight[i] ) continue;
            incoming.record(Nu[i]);
        }

        outgoing.free();
        outgoing.record(coeff);
        std::cerr << "-- incoming: " << incoming << std::endl;
        std::cerr << "-- outgoing: " << outgoing << std::endl;

        const size_t outgoingSize = outgoing.size();
        const size_t incomingSize = incoming.size();

        switch( Sign::Of(outgoingSize,incomingSize) )
        {
            case Negative:
                break;

            case __Zero__:
                if(outgoing!=incoming) throw Exception("same size but different content!");
                continue;

            case Positive:
                throw Exception("More Outgoing Than Incoming");
        }
        incoming ^= outgoing;

        std::cerr << "-- missing: " << incoming << std::endl;


    }


}
Y_UDONE()

