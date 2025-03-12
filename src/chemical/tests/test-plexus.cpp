
#include "y/chemical/type/meta-list.hpp"
#include "y/chemical/weasel.hpp"

#include "y/utest/run.hpp"

using namespace Yttrium;
using namespace Chemical;


Y_UTEST(plexus)
{

    Weasel &weasel = Weasel::Instance();
    weasel << "function f(t) return 1.1 end";

    Library    lib;
    Equilibria eqs;

    for(int iarg=1;iarg<argc;++iarg)
    {
        const String data = argv[iarg];
        weasel(lib,eqs,Lingo::Module::Open(data,data) );
    }

    std::cerr << "lib=" << lib << std::endl;
    std::cerr << "eqs=" << eqs << std::endl;

    ParaList<SubLevel,SList> msl;
    ParaList<AuxLevel,EList> mel;

    for(Library::ConstIterator it=lib->begin();it!=lib->end();++it)
    {
        const Species &sp = **it;
        msl << sp;
    }

    for(Equilibria::Iterator it=eqs.begin();it!=eqs.end();++it)
    {
        Equilibrium &eq = **it;
        mel << eq;
    }
}
Y_UDONE()

