
#include "y/chemical/weasel/compiler.hpp"
#include "y/utest/run.hpp"
#include "y/chemical/reactive/aftermath.hpp"
#include "y/sequence/vector.hpp"

//#include "y/stream/libc/output.hpp"

using namespace Yttrium;
using namespace Chemical;

Y_UTEST(weasel)
{

    Weasel::Compiler &weasel = Weasel::Compiler::Instance();

    Library        lib;
    LuaEquilibria  eqs;

    for(int i=1;i<argc;++i)
    {
        weasel(lib,eqs,Jive::Module::OpenData(argv[i],argv[i]));
    }

    std::cerr << "lib=" << lib << std::endl;
    std::cerr << "eqs=" << eqs << std::endl;

    const size_t    m = lib->size();
    Vector<xreal_t> C0(m,0.01);
    Vector<xreal_t> C(m,0);

    std::cerr << "C0=" << C0 << std::endl;

    Aftermath       am;
    for(Equilibria::Iterator it=eqs.begin();it!=eqs.end();++it)
    {
        Equilibrium &eq = **it;
        std::cerr << eq << " kind=" << eq.kind << std::endl;
        am.solve(C,TopLevel, C0, TopLevel, eq, eq.K(0) );
        std::cerr << "C=" << C << std::endl;
    }



}
Y_UDONE()
