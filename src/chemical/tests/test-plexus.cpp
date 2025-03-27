
#include "y/chemical/plexus/reactors.hpp"
#include "y/chemical/plexus/conservation/broken.hpp"

#include "y/chemical/weasel.hpp"
#include "y/utest/run.hpp"
#include "y/random/mt19937.hpp"
#include "y/stream/libc/output.hpp"

#include "y/data/small/heavy/list/coop.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        namespace Conservation
        {

           

            class Warden
            {
            public:

                explicit Warden(const Cluster &_cluster,
                                const Canon   &_canon) :
                cluster(_cluster),
                canon(_canon),
                xadd(),
                blist(canon.size),
                cproj(canon.size+1,canon.species->size),
                c0(cproj[canon.size+1]),
                injected(canon.species->size)
                {
                }

                virtual ~Warden() noexcept
                {
                }

                void operator()(XMLog      &xml,
                                XWritable  &C0,
                                const Level L0);

                std::ostream & display(std::ostream &os, const Broken &b) const
                {
                    canon.pad(os << b.law.name,b.law) << " | " << std::setw(22) << b.xs.str();
                    return os;
                }

                const Cluster &cluster;
                const Canon   &canon;
                XAdd           xadd;
                BList          blist;
                XMatrix        cproj;
                XWritable     &c0;
                Summator       injected;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Warden);
            };

            static inline SignType CompareBroken(const BNode * const lhs,
                                                 const BNode * const rhs) noexcept
            {
                return Sign::Of( (**lhs).xs, (**rhs).xs);
            }

            void Warden:: operator()(XMLog     & xml,
                                     XWritable & C0,
                                     const Level L0)
            {
                Y_XML_SECTION_OPT(xml, "Warden", "|canon|=" << canon.size);
                const xreal_t zero;

                injected.forEach(& XAdd::free );
                {
                    Y_XML_SECTION(xml,"Initialize");
                    //----------------------------------------------------------
                    //
                    //
                    // populate list of broken laws
                    //
                    //
                    //----------------------------------------------------------
                    blist.free();
                    canon.transfer(c0,AuxLevel,C0,L0);

                    for(const LNode *ln=canon.head;ln;ln=ln->next)
                    {
                        const Law &   law = **ln;
                        const xreal_t xs  = law.excess(xadd,c0,AuxLevel);
                        if(xs>zero)
                        {
                            XWritable &cc = cproj[blist.size+1].ld(c0);
                            law.project(xadd,cc,c0,AuxLevel);
                            const Broken broken(law,xs,cc);
                            blist << broken;
                            if(xml.verbose) display(   xml() << "[broken] ", broken) << std::endl;
                        }
                        else {
                            if(xml.verbose) canon.pad( xml() << "[ -ok- ] " << law.name,law) << std::endl;
                        }
                    }
                }


                {
                    //----------------------------------------------------------
                    //
                    //
                    // reduce list of broken laws by iterative mending
                    //
                    //
                    //----------------------------------------------------------
                    size_t iter = 0;
                    while(blist.size>0)
                    {
                        Y_XML_SECTION_OPT(xml, "MendBroken", "iteration=" << ++iter);

                        //------------------------------------------------------
                        //
                        // find the smallest increase
                        //
                        //------------------------------------------------------
                        MergeSort::Call(blist,CompareBroken);
                        {
                            Broken &best = **blist.head;
                            Y_XML_COMMENT(xml,"best: " << best.law.name);
                            const Law &law = best.law;

                            // set c0 to new value
                            c0.ld(best.cc);

                            // update injected
                            for(const Actor *a=law->head;a;a=a->next)
                            {
                                const xreal_t delta = (a->xn * best.xs) / law.denom;
                                a->sp(injected,AuxLevel) << delta;
                            }
                        }

                        //------------------------------------------------------
                        //
                        // keep still broken laws
                        //
                        //------------------------------------------------------
                        blist.cutHead(); // discard
                        for(BNode *node=blist.head;node;)
                        {
                            BNode * const next = node->next;
                            Broken &      curr = **node;
                            curr.xs = curr.law.excess(xadd,c0,AuxLevel);
                            if(curr.xs<=zero)
                            {
                                // drop
                                Y_XML_COMMENT(xml, "drop: " << curr.law.name);
                                blist.cutNode(node);
                                node=0;
                            }
                            else
                            {
                                // keep with RELOADING before projection
                                Y_XML_COMMENT(xml, "keep: " << curr.law.name);
                                curr.law.project(xadd, curr.cc.ld(c0), c0, AuxLevel );
                            }
                            node=next;
                        }

                        if(xml.verbose)
                        {
                            for(const BNode *node=blist.head;node;node=node->next)
                            {
                                display(   xml() << "[broken] ", **node) << std::endl;
                            }
                        }
                    }
                }

                if(xml.verbose)
                {
                    canon.show( xml() << "injected=",injected);
                }

                canon.transfer(C0,L0,c0,AuxLevel);

            }
        }
    }
}

using namespace Yttrium;
using namespace Chemical;



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

    bool verbose = true;

    XMLog          xml(verbose);
    Clusters       cls(xml,eqs,0.0);

    std::cerr << "lib=" << lib << std::endl;
    cls.graphViz("cs");

    const size_t m = lib->size();
    XVector      C0(m,0);
    XVector      C(m,0);



    Library::Concentrations(C0,ran,0.5,0.5);
    lib.show(std::cerr << "C0=", "\t[", C0, "]", xreal_t::ToString ) << std::endl;

    for(const Cluster *cl=cls->head;cl;cl=cl->next)
    {
        for(const Conservation::Canon *canon=cl->canons.head;canon;canon=canon->next)
        {
            Conservation::Warden warden(*cl,*canon);
            warden(xml,C0,TopLevel);
        }
    }

    lib.show(std::cerr << "C0=", "\t[", C0, "]", xreal_t::ToString ) << std::endl;

    return 0;

    Reactor::MonitorScore = true;
    Reactor::EmitProfiles = true;
    Reactors cs(cls);
    cs(xml,C0);




    
    Y_SIZEOF(Cluster);
    Y_SIZEOF(ClusterContent);

    Y_SIZEOF(Conservation::Law);
    Y_SIZEOF(Conservation::Laws);
    Y_SIZEOF(Conservation::Canon);
    Y_SIZEOF(Conservation::Canons);

    Y_SIZEOF(Reactor);


}
Y_UDONE()

