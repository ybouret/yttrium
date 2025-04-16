#include "y/chemical/plexus/reactors.hpp"
#include "y/chemical/plexus/equalizer.hpp"

#include "y/chemical/weasel.hpp"
#include "y/utest/run.hpp"
#include "y/random/mt19937.hpp"
#include "y/stream/libc/output.hpp"

#include "y/string/env.hpp"


namespace Yttrium
{
    namespace Chemical
    {

        namespace Initial
        {

            class Axiom
            {
            protected:
                explicit Axiom(const xreal_t value) noexcept;

            public:
                virtual ~Axiom() noexcept;

                virtual int   weight(const Species &sp) const noexcept = 0;
                const xreal_t amount;

                Axiom *next;
                Axiom *prev;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Axiom);
            };

            Axiom:: Axiom(const xreal_t value) noexcept :
            amount(value),
            next(0),
            prev(0)
            {
            }

            Axiom:: ~Axiom() noexcept
            {
            }

            class FixedConcentration : public Axiom
            {
            public:
                explicit FixedConcentration(const Species &sp,
                                            const xreal_t  C0) noexcept;
                virtual ~FixedConcentration() noexcept;

                virtual int weight(const Species &sp) const noexcept;


                const Species &species;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(FixedConcentration);
            };


            FixedConcentration:: ~FixedConcentration() noexcept
            {
            }

            FixedConcentration:: FixedConcentration(const Species &sp,
                                                    const xreal_t  C0) noexcept :
            Axiom(C0),
            species(sp)
            {
            }


            int FixedConcentration:: weight(const Species &sp) const noexcept
            {
                return ( &sp == &species ) ? 1 : 0;
            }



            class ElectroNeutrality : public Axiom
            {
            public:
                explicit ElectroNeutrality() noexcept;
                virtual ~ElectroNeutrality() noexcept;

                virtual int weight(const Species &sp) const noexcept;


            private:
                Y_DISABLE_COPY_AND_ASSIGN(ElectroNeutrality);
            };


            ElectroNeutrality:: ~ElectroNeutrality() noexcept
            {
            }

            ElectroNeutrality:: ElectroNeutrality() noexcept :
            Axiom(0)
            {
            }


            int ElectroNeutrality:: weight(const Species &sp) const noexcept
            {
                return sp.z;
            }




        }

    }
}


using namespace Yttrium;
using namespace Chemical;

namespace
{
    struct Display
    {
        void operator()(const XReadable &C0,
                        const xreal_t    S0)
        {
            std::cerr << "score=" << std::setw(24) << S0.str() << " @" << C0 << std::endl;
        }
    };
}

Y_UTEST(plexus)
{

    Random::MT19937 ran;
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

    bool           verbose = Environment::Flag("VERBOSE");
    XMLog          xml(verbose);
    Clusters       cls(xml,eqs,0.0);


    
    std::cerr << "lib=" << lib << std::endl;


    cls.graphViz("cs");


    const size_t m = lib->size();
    XVector      C0(m,0); // concentration

    Library::Concentrations(C0,ran,0.1,0.5);
    lib.show(std::cerr << "C0=", "\t[", C0, "]", xreal_t::ToString ) << std::endl;

    Equalizer eqz(cls);
    eqz(xml,C0);

    lib.show(std::cerr << "C1=", "\t[", C0, "]", xreal_t::ToString ) << std::endl;




    Reactor::MonitorScore = true;
    Reactor::EmitProfiles = true;
    Reactor::EmitProfiles = false;
    Reactors cs(cls);

    Display            display;
    Reactor::Proc cb = display;
    cs(xml,C0,&cb);

    lib.show(std::cerr << "C2=", "\t[", C0, "]", xreal_t::ToString ) << std::endl;

    
}
Y_UDONE()

