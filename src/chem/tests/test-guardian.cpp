
#include "y/chem/weasel.hpp"
#include "y/chem/plexus.hpp"
#include "y/chem/plexus/conservation/guardian.hpp"

#include "y/utest/run.hpp"

using namespace Yttrium;


Y_UTEST(guardian)
{
    Random::Rand            ran;
    Chemical::Weasel       &wsl = Chemical::Weasel::Instance();
    Chemical::Library       lib;
    Chemical::LuaEquilibria eqs;

    for(int i=1;i<argc;++i)
    {
        const String data = argv[i];
        wsl( Jive::Module::OpenData(data,data), lib, eqs );
    }


    std::cerr << "lib=" << lib << std::endl;
    std::cerr << "eqs=" << eqs << std::endl;

    bool             verbose = true;
    XMLog            xml(verbose);
    Chemical::Plexus plexus(eqs,xml,0.0);

    std::cerr << "eqs=" << eqs << std::endl;

    Vector<Chemical::xreal> C0(lib->size(),0), Cerr(lib->size(),0);
    lib.fill(C0, ran);
    for(size_t i=C0.size();i>0;--i)
    {
        if( ran.to<double>() > 0.7 ) C0[i] = -C0[i];
    }
    lib(std::cerr << "Corg=","  [",C0,"]",Chemical::Conv::X2R);


    Chemical::Guardian      guardian;
    const Chemical::SpList &corrected =  guardian(plexus,C0,Cerr,xml);


    lib(std::cerr << "Corg=","  [",C0,  "]",Chemical::Conv::X2R);
    lib(std::cerr << "Cerr=","  [",Cerr,"]",Chemical::Conv::X2R);
    std::cerr << "corrected = " << corrected << std::endl;


}
Y_UDONE()

