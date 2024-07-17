
#include "y/chemical/weasel/compiler.hpp"
#include "y/utest/run.hpp"
#include "y/chemical/reactive/aftermath.hpp"
#include "y/sequence/vector.hpp"
#include "y/random/park-miller.hpp"
#include "y/stream/libc/output.hpp"
#include "y/graphviz/color-scheme.hpp"
#include "y/mkl/tao/seq/level1.hpp"

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
    Vector<xreal_t> dC(m,0);

    Species::Conc(C0,ran,0.3);
    std::cerr << "C0=" << C0 << std::endl;
    

    Aftermath       am;
    XAdd            xadd(m);
    for(Equilibria::Iterator it=eqs.begin();it!=eqs.end();++it)
    {
        Equilibrium &eq = **it;
        std::cerr << eq << " kind=" << eq.kind << std::endl;
        const String dotFile = eq.name+".dot";
        {
            OutputFile fp(dotFile);

            GraphViz::Vizible::Enter(fp, "G");
            const String color = "black";

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
            const xreal_t xi = am.eval(dC,C, TopLevel, C0, TopLevel, eq);
            std::cerr << "xi=" << xi << std::endl;
            eq.drvsMassAction(K, phi, TopLevel, C, TopLevel, am.xmul);
            std::cerr << "phi=" << phi << std::endl;
            std::cerr << "dC =" << dC  << std::endl;

            const String fileName = eq.name + ".dat";
            OutputFile   fp(fileName);
            const xreal_t sigma1 = MKL::Tao::DotProduct<xreal_t>::Of_(phi,Nu,xadd);
            std::cerr << "sigma1=" << sigma1 << std::endl;
            std::cerr << "ma0   =" << eq.massAction(K, am.xmul, C0, TopLevel) << std::endl;

            const size_t np = 100;
            for(size_t i=0;i<=np;++i)
            {
                const xreal_t u  = xreal_t(i)/xreal_t(np);
                const xreal_t x  = (xreal_t(i)*xi)/xreal_t(np);
                const xreal_t ma = eq.massAction(K, am.xmul, C0, TopLevel, x);
                eq.addSafe(C, TopLevel, C0, TopLevel, x);
                eq.drvsMassAction(K, phi, C, TopLevel, am.xmul, C0, TopLevel,  x);
                const xreal_t sigma = MKL::Tao::DotProduct<xreal_t>::Of_(phi,Nu,xadd);
                //std::cerr << "sigma=" << real_t(sigma) << std::endl;

                fp("%.15g %.15g %.15g\n", real_t(x), real_t(ma), real_t(sigma));
            }

        }
        else
        {
            std::cerr << "inactive" << std::endl;
            eq.drvsMassAction(K, phi, TopLevel, C0, TopLevel, am.xmul);
            std::cerr << "phi=" << phi << std::endl;
        }
        
    }



}
Y_UDONE()
