


#include "y/chem/weasel.hpp"
#include "y/chem/plexus/equalizer.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;


Y_UTEST(equalizer)
{
    Chemical::Weasel       &wsl = Chemical::Weasel::Instance();
    Chemical::Library       lib;
    Chemical::LuaEquilibria eqs;
    Random::Rand            ran;

    for(int i=1;i<argc;++i)
    {
        const String data = argv[i];
        wsl( Jive::Module::OpenData(data,data), lib, eqs );
    }


    std::cerr << "lib=" << lib << std::endl;
    std::cerr << "eqs=" << eqs << std::endl;

    bool  verbose = true;
    XMLog xml(verbose);
    Chemical::Plexus plexus(eqs,xml,0.0);

    std::cerr << eqs << std::endl;

    plexus.graphViz();


    Chemical::Custodian custodian;
    custodian.prepare(plexus,xml);
    custodian.startup();

    Vector<Chemical::xreal> Corg(lib->size(),0), Cerr(lib->size(),0);
    lib.fill(Corg, ran);
    for(size_t i=Corg.size();i>0;--i)
    {
        if( ran.to<double>() > 0.1 ) Corg[i] = -Corg[i];
    }
    lib(std::cerr << "Corg=","  [",Corg,"]",Chemical::Conv::X2R);

    if(true)
    {
        custodian(plexus,Corg,xml);
        custodian.query(xml,lib);
        const Chemical::SpList &modified = custodian.collect(Cerr);
        std::cerr << "modified: " << modified  << std::endl;
        lib(std::cerr << "Cerr=","  [",Cerr,"]",Chemical::Conv::X2R);
        lib(std::cerr << "Corg=","  [",Corg,"]",Chemical::Conv::X2R);
    }

    Chemical::Equalizer equalizer;
    equalizer.prepare(plexus,xml);
    
    for(const Chemical::Cluster *cluster=plexus->head;cluster;cluster=cluster->next)
    {
        equalizer.run(*cluster, Corg, xml);
    }


}
Y_UDONE()

