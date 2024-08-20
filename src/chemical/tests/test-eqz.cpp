#include "y/chemical/plexus.hpp"
#include "y/chemical/plexus/equalizer/faders.hpp"

#include "y/utest/run.hpp"

using namespace Yttrium;
using namespace Chemical;

namespace Yttrium
{
    namespace Chemical
    {

       


        class Altered
        {
        public:
            typedef CxxSeries<Altered,XMemory> Series;

            explicit Altered(const Equilibrium & _eq,
                             const XReadable   & _cc,
                             const xreal_t       _gg) noexcept :
            eq(_eq),
            cc(_cc),
            gg(_gg)
            {
            }

            Altered(const Altered &_) noexcept :
            eq(_.eq),
            cc(_.cc),
            gg(_.gg)
            {
            }


            ~Altered() noexcept {}

            const Equilibrium & eq;
            const XReadable   & cc;
            const xreal_t       gg;

        private:
            Y_DISABLE_ASSIGN(Altered);
        };

        class Equalizer
        {
        public:
            explicit Equalizer(const Cluster &cl) :
            rcl(cl),
            neq(cl.size),
            nsp(cl.species.size),
            xadd(nsp*2),
            banks(),
            best(banks.s),
            faders(neq,CopyOf,banks),
            ceq(neq,nsp),
            altered(neq)
            {
            }

            virtual ~Equalizer() noexcept {}


            void run(XWritable &C, const Level L, XMLog &xml)
            {
                Y_XML_SECTION(xml, "Eqz");
                const xreal_t      zero;
                const AddressBook &book = rcl.conserved.book;

                altered.free();
                for(const ENode *en=rcl.limited.head;en;en=en->next)
                {
                    const Equilibrium &eq = **en;
                    Faders            &fd = faders[ eq.indx[SubLevel] ];
                    const unsigned     id = fd(C,L,eq,book);
                    const size_t       ii = altered.size() + 1;
                    XWritable         &cc = ceq[ii];
                    rcl.transfer(cc,SubLevel,C,L);

                    if(xml.verbose)
                    {
                        rcl.uuid.pad( xml() << Faders::TextFlag(id) << " | " << eq.name, eq) << std::endl;
                        if(id!=Faders::BALANCED)
                        {
                            xml() << "\treac: " << fd.reac << std::endl;
                            xml() << "\tprod: " << fd.prod << std::endl;
                        }
                    }

                    switch(id)
                    {
                        case Faders::BALANCED:
                            continue;

                        case Faders::BAD_BOTH:
                            altered << Altered(eq,cc,zero);
                            continue;

                        case Faders::BAD_PROD: {
                            // need a forward alteration
                            assert(fd.prod.required.size>0);
                            assert(fd.reac.limiting.size>0);
                            if(bestEffort(fd.reac.limiting,fd.prod.required))
                            {
                                Y_XMLOG(xml, "\tbest: " << best);
                                bestAlter(eq,cc);
                            }
                            else
                            {
                                Y_XMLOG(xml, "\tno best effort");
                            }
                        } continue;

                        case Faders::BAD_REAC: {
                            // need a reverse alteration
                            assert(fd.reac.required.size>0);
                            assert(fd.prod.limiting.size>0);
                            if(bestEffort(fd.prod.limiting,fd.reac.required))
                            {
                                best.xi = -best.xi;
                                Y_XMLOG(xml, "\tbest: " << best);
                                bestAlter(eq,cc);
                            }
                            else
                            {
                                Y_XMLOG(xml, "\tno best effort");
                            }
                        }  continue;
                    }
                    throw Exception("bad id");
                }
            }

            void bestAlter(const Equilibrium &eq, 
                           XWritable         &cc)
            {
                const xreal_t              zero;
                const xreal_t              xi = best.xi; assert( xi.abs() > zero );
                Equilibrium::ConstIterator it = eq->begin();

                xadd.free();
                for(size_t j=eq->size();j>0;--j,++it)
                {
                    const Component &cm = **it;
                    const Species   &sp = cm.sp;
                    const xreal_t    dc = xi * cm.xn;
                    const size_t     jj = sp.indx[SubLevel];
                    const xreal_t    cOld = cc[jj];
                    if(cOld<zero)
                    {
                        xadd << -cOld;
                    }
                    const xreal_t cNew = (cc[jj] = cOld + dc);
                    if(cNew<zero)
                    {
                        xadd << cNew;
                    }

                }

                for(const SNode *node=best.head;node;node=node->next)
                {
                    const Species &sp = **node;
                    cc[ sp.indx[SubLevel] ] = zero;
                }
                const xreal_t gg = xadd.sum();
                eq.displayCompact(std::cerr << "\t\t", cc, SubLevel) << std::endl;
                std::cerr << "\t\tgain=" << real_t(gg) << std::endl;
                altered << Altered(eq,cc,gg);
            }


            bool bestEffort(const Boundary   &limiting,
                            const Boundaries &required)
            {
                best.empty();
                assert(limiting.size>0); assert(limiting.xi>=0.0);
                assert(required.size>0);

                const xreal_t    ximax = limiting.xi;
                const Boundary  *lower = 0;

                for(const BNode *node  = required.head;node;node=node->next)
                {
                    const Boundary &here = **node;
                    switch(Sign::Of(here.xi,ximax))
                    {
                        case Negative:
                            lower = &here; // record for later
                            continue;

                        case __Zero__:
                            best.xi = ximax;   // numerical match
                            best << here;      // load vanishing in here
                            best << limiting;  // load vanishing in limiting
                            return true;

                        case Positive:
                            goto DONE; // no more possibility

                    }
                }

            DONE:
                if(0 != lower)
                {
                    const Boundary &here = *lower;
                    best.xi = here.xi;
                    best << here;
                    return true;
                }
                else
                {
                    if(ximax>0.0)
                    {
                        best.xi = ximax;
                        best << limiting;
                        return true;
                    }
                    else
                        return false;
                }
            }

            const Cluster &   rcl;
            const size_t      neq;
            const size_t      nsp;
            XAdd              xadd;
            Banks             banks;
            Boundary          best;
            Faders::Array     faders;
            XMatrix           ceq;
            Altered::Series   altered;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Equalizer);
        };

    }

}

Y_UTEST(eqz)
{
    Plexus plexus(true);

    for(int i=1;i<argc;++i)
    {
        plexus(Jive::Module::OpenData(argv[i],argv[i]));
    }

    std::cerr << "lib=" << plexus.lib << std::endl;
    std::cerr << "eqs=" << plexus.eqs << std::endl;

    const Library   &lib = plexus.lib;
    Clusters        &cls = plexus.assemble();
    //const XReadable &K   = cls.K(0);
    XVector C0(lib->size(),0);


    for(const Cluster *cl=cls->head;cl;cl=cl->next)
    {
        Equalizer eqz(*cl);

        plexus.conc(C0,0.3,0.5);
        lib(std::cerr << "C0=","\t[",C0,"]");

        eqz.run(C0, TopLevel, plexus.xml);

#if 0
        for(const ENode *en=cl->head;en;en=en->next)
        {
            const Equilibrium &eq = **en;
            F(C0,TopLevel,eq,cl->conserved.book);
            cl->uuid.pad(std::cerr << eq.name, eq) << ":" << F << std::endl;
        }
#endif

        //lib(std::cerr << "C1=","\t[",C0,"]");



    }




    Y_SIZEOF(Boundary);
    Y_SIZEOF(Boundaries);
}
Y_UDONE()
