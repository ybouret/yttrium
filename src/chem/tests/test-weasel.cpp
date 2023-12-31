#include "y/chem/weasel.hpp"
#include "y/container/matrix.hpp"
#include "y/utest/run.hpp"
#include "y/chem/algebraic.hpp"

using namespace Yttrium;


Y_UTEST(weasel)
{
    Chemical::Weasel       &wsl = Chemical::Weasel::Instance();
    Chemical::Library       lib;
    Chemical::LuaEquilibria eqs;

    wsl.verbose = true;
    if(argc>1)
    {
        wsl( Jive::Module::OpenFile(argv[1]), lib, eqs);
    }

    std::cerr << "lib=" << lib << std::endl;
    std::cerr << "eqs=" << eqs << std::endl;

    if( eqs->size() > 0 )
    {
        Y_ASSERT(lib->size()>0);
        const size_t N = eqs->size();
        const size_t M = lib->size();
        Matrix<int>  Nu(N,M);
        for(Chemical::Equilibria::Iterator it = eqs.begin();it!=eqs.end();++it)
        {
            Chemical::Equilibrium &eq = **it;
            eq.fill(Nu[eq.indx[Chemical::TopLevel]], Chemical::TopLevel);
        }
        std::cerr << "Nu=" << Nu << std::endl;
        bool  verbose = true;
        XMLog xml(verbose);

        Matrix<unsigned> Q;
        Chemical::Algebraic::Compute(Q,Nu,xml);
 
        Chemical::Algebraic::Weight::List W;
        Chemical::Algebraic::Compute(W,Nu,xml);
        std::cerr << "W=" << std::endl;
        for(Chemical::Algebraic::Weight *w=W.head;w;w=w->next)
        {
            std::cerr << *w << " => " << w->stoi << std::endl;
        }
        std::cerr << "Q=" << Q << std::endl;
        
    }

    Y_SIZEOF(Chemical::Equilibria);

}
Y_UDONE()

