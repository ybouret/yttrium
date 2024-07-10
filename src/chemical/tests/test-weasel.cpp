
#include "y/chemical/weasel/compiler.hpp"
#include "y/utest/run.hpp"
#include "y/chemical/reactive/aftermath.hpp"
#include "y/sequence/vector.hpp"
#include "y/random/park-miller.hpp"
#include "y/stream/libc/output.hpp"
#include "y/graphviz/color-scheme.hpp"

using namespace Yttrium;
using namespace Chemical;

Y_UTEST(weasel)
{
    Weasel::Compiler &weasel = Weasel::Compiler::Instance();

    Random::ParkMiller ran;
    Library            lib;
    LuaEquilibria      eqs;

    for(int i=1;i<argc;++i)
    {
        weasel(lib,eqs,Jive::Module::OpenData(argv[i],argv[i]));
    }

    std::cerr << "lib=" << lib << std::endl;
    std::cerr << "eqs=" << eqs << std::endl;

    const size_t    m = lib->size();
    Vector<xreal_t> C0(m,0.01);
    Vector<xreal_t> C(m,0);
    Vector<xreal_t> phi(m,0);
    Vector<int>     Nu(m,0);

    Species::Conc(C0,ran,0.3);
    std::cerr << "C0=" << C0 << std::endl;
    

    Aftermath       am;
    for(Equilibria::Iterator it=eqs.begin();it!=eqs.end();++it)
    {
        Equilibrium &eq = **it;
        std::cerr << eq << " kind=" << eq.kind << std::endl;
        const String dotFile = eq.name+".dot";
        {
            OutputFile fp(dotFile);

            GraphViz::Vizible::Enter(fp, "G");
            {
                const String color = GraphViz::Vizible::Color("set18", 1);
                for(Library::ConstIterator it=lib->begin();it!=lib->end();++it)
                {
                    (**it).viz(fp, color, true);
                }
            }

            {
                const String color = GraphViz::Vizible::Color("set18", 2);
                eq.viz(fp,color);
#if 0
                for(const Actor *a=eq.reac->head;a;a=a->next)
                {
                    a->viz(fp, eq, color, Iterating::Forward);
                }
                for(const Actor *a=eq.prod->head;a;a=a->next)
                {
                    a->viz(fp, eq, color, Iterating::Reverse);
                }
#endif
            }

            GraphViz::Vizible::Leave(fp);
        }
        GraphViz::Vizible::RenderPNG(dotFile,true);


        eq.topology(Nu, TopLevel);
        std::cerr << "Nu=" << Nu << std::endl;
        const xreal_t K = eq.K(0);
        if(am.solve(C,TopLevel, C0, TopLevel, eq, K ))
        {
            std::cerr << "C="  << C << std::endl;
            std::cerr << "Xi=" << eq.massAction(K, am.xmul, C, TopLevel) << std::endl;
            const xreal_t xi = am.eval(C, TopLevel, C0, TopLevel, eq);
            std::cerr << "xi=" << xi << std::endl;
            eq.drvsMassAction(K, phi, TopLevel, C, TopLevel, am.xmul);
            std::cerr << "phi=" << phi << std::endl;
        }
        else
        {
            std::cerr << "inactive" << std::endl;
        }
        
    }



}
Y_UDONE()
