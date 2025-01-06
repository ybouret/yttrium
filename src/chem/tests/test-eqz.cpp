

#include "y/chemical/plexus/mixes.hpp"

#include "y/chemical/library.hpp"
#include "y/random/park-miller.hpp"



#include "y/utest/run.hpp"
#include "y/stream/libc/output.hpp"

namespace Yttrium
{

    namespace Chemical
    {

        class Requests : public Cursors
        {
        public:
            explicit Requests(const XBanks &_) noexcept : Cursors(_) {}
            virtual ~Requests() noexcept {}
            Requests(const Requests &_) : Cursors(_) {}

        private:
            Y_DISABLE_ASSIGN(Requests);
        };


        class Limiting : public Marker
        {
        public:
            explicit Limiting(const XBanks &_) : Marker(_.species) {}
            explicit Limiting(const Limiting &_) : Marker(_) {}
            virtual ~Limiting() noexcept {}

            void update(const Species &sp, const xReal xi)
            {
                assert(xi>=0.0);
                if(srepo.size<=0)
                {
                    srepo << sp;
                    value = xi;
                }
                else
                {
                    switch( Sign::Of(xi,value) )
                    {
                        case Negative:
                            // new winner
                            srepo.free();
                            srepo << sp;
                            value = xi;
                            break;

                        case __Zero__:
                            // group same extent
                            srepo << sp;
                            break;


                        case Positive:
                            // discard
                            break;
                    }
                }
            }


        private:
            Y_DISABLE_ASSIGN(Limiting);

        };

        class Boundaries
        {
        public:
            Boundaries(const XBanks &_) noexcept :
            limiting(_),
            requests(_)
            {
            }

            ~Boundaries() noexcept {}
            Boundaries(const Boundaries &_) : limiting(_.limiting), requests(_.requests) {}

            Limiting limiting;
            Requests requests;


        private:
            Y_DISABLE_ASSIGN(Boundaries);
        };

    }

}

using namespace Yttrium;
using namespace Chemical;


Y_UTEST(eqz)
{
    Random::ParkMiller ran;
    Library            lib;
    Equilibria         eqs;

    for(int i=1;i<argc;++i)
    {
        eqs(lib,argv[i]);
    }

    std::cerr << "lib=" << lib << std::endl;
    std::cerr << "eqs=" << eqs << std::endl;

    bool     verbose = true;
    XMLog    xml(verbose);
    Mixes    mixes(xml,eqs);

    std::cerr << "mixes=" << mixes << std::endl;
    GraphViz::Vizible::DotToPngEx("warden.dot",mixes,1);

    const size_t M = lib->size();
    XVector      C0(M,0);
    XVector      dC(M,0);

    Library::Conc(C0,ran,0.5,0.5);

    lib(std::cerr << "C0=","[",C0,"]", xReal::ToString) << std::endl << std::endl;
    
#if 0
    std::cerr << "Wardens..." << std::endl;
    for(const Mix *mix=mixes->head;mix;mix=mix->next)
    {
        if(mix->auth.isEmpty()) continue;

        for(const Conservation::Act *act=mix->auth->acts->head;act;act=act->next)
        {
            Conservation::Warden warden(*act);
            warden.run(xml,C0,TopLevel,dC);
        }

    }
#endif
    

}
Y_UDONE()
