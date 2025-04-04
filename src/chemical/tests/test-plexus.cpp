
#include "y/chemical/plexus/reactors.hpp"
#include "y/chemical/plexus/conservation/wardens.hpp"

#include "y/chemical/weasel.hpp"
#include "y/utest/run.hpp"
#include "y/random/mt19937.hpp"
#include "y/stream/libc/output.hpp"

#include "y/chemical/plexus/equalizer/extents.hpp"

#include "y/chemical/plexus/conservation/broken.hpp"


namespace Yttrium
{
    namespace Chemical
    {
#if 0
        class Gain
        {
        public:
            Gain(const xreal_t      _g,
                 const Components & _E,
                 const XReadable  & _c) noexcept :
            g(_g), E(_E), C(_c)
            {
            }

            ~Gain() noexcept {}

            Gain(const Gain &_) noexcept :
            g(_.g), E(_.E), C(_.C)
            {
            }

            Y_OSTREAM_PROTO(Gain);

            const xreal_t     g;
            const Components &E;
            const XReadable  &C;

        private:
            Y_DISABLE_ASSIGN(Gain);
        };

        std::ostream & operator<<(std::ostream &os, const Gain &gain)
        {
            os << std::setw(Restartable::Width) << gain.g.str() << " @" << gain.E.name << "=";
            gain.E.displayCompact(os,gain.C,SubLevel);
            return os;
        }

        typedef Small::CoopHeavyList<Gain>  GList_;
        typedef GList_::NodeType            GNode;
        typedef GList_::ProxyType           GBank;

        class KList : public ESolo
        {
        public:
            explicit KList() noexcept : ESolo() {}
            virtual ~KList() noexcept {}

            void show(XMLog &xml, const char * const uuid)
            {
                if(!xml.verbose) return;
                Y_XML_SECTION_OPT(xml,"Gain","is " << uuid);
                for(const ENode *gn=head;gn;gn=gn->next)
                {
                    Y_XMLOG(xml, (**gn).name);
                }
            }
        private:
            Y_DISABLE_COPY_AND_ASSIGN(KList);
        };

        class GList : public GList_
        {
        public:
            explicit GList(const GBank &_) noexcept : GList_(_) {}
            virtual ~GList() noexcept {}

            void show(XMLog &xml, const char * const uuid)
            {
                if(!xml.verbose) return;
                Y_XML_SECTION_OPT(xml,"Gain","is " << uuid);
                for(const GNode *gn=head;gn;gn=gn->next)
                {
                    Y_XMLOG(xml, **gn);
                }
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(GList);
        };

        class Equalizer
        {
        public:
            explicit Equalizer(const Cluster &cls);
            virtual ~Equalizer() noexcept;


            const Cluster &cluster;
            EqzBanks       banks;
            const size_t   nrows;
            const size_t   ncols;
            Extents        extents;
            KList          klist;
            GBank          gbank;
            GList          zgain;
            GList          pgain;
            XMatrix        c_eqz;
            XAdd           xadd;

            void operator()(XMLog &xml, XWritable &C0);

            void analyze(XMLog &xml, const Extents &, const XReadable &C0, const AddressBook * wanders);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Equalizer);
        };


        Equalizer:: Equalizer(const Cluster &cls) :
        cluster(cls),
        banks(),
        nrows(cluster.definite->size),
        ncols(cluster->species->size),
        extents(banks),
        klist(),
        gbank(),
        zgain(gbank),
        pgain(gbank),
        c_eqz(nrows,nrows>0?ncols:nrows),
        xadd()
        {
            gbank->reserve(nrows);
        }

        Equalizer:: ~Equalizer() noexcept
        {
        }


        //! decreasing
        static inline
        SignType CompareGains(const GNode * const lhs,
                              const GNode * const rhs) noexcept
        {
            const Gain &L = **lhs;
            const Gain &R = **rhs;
            switch( Sign::Of(L.g,R.g) )
            {
                case Positive: return Negative;
                case Negative: return Positive;
                case __Zero__: break;
            }
            return Sign::Of(L.E.indx[TopLevel],R.E.indx[TopLevel]);
        }

        void Equalizer:: operator()(XMLog     &xml,
                                    XWritable &C0)
        {
            Y_XML_SECTION(xml, "Equalizer");

            Y_XML_COMMENT(xml,"definite");
            do
            {
                klist.free();
                zgain.free();
                pgain.free();

                for(const ENode *en=cluster.definite->head;en;en=en->next)
                {
                    const Equilibrium &eq   = **en;
                    //Y_XML_SECTION(xml,*eq.name);
                    const Resultant    res  = extents(xml,eq,C0,TopLevel, & cluster.wandering );

                    switch(res)
                    {
                        case Correct: continue;
                        case BadBoth: klist << eq; continue;
                        case BadReac: break;
                        case BadProd: break;
                    }

                    XWritable &cc = c_eqz[zgain.size+pgain.size+1];
                    cluster.gather(cc,C0);
                    const Gain G(extents.generate(xml,xadd, cc, eq, C0, TopLevel, &cluster.wandering),eq,cc);
                    assert(G.g.mantissa>=0);
                    if(G.g.mantissa<=0) zgain << G; else pgain << G;
                }

                {
                    Y_XML_SECTION(xml,"Status");
                    MergeSort::Call(pgain,CompareGains);
                    klist.show(xml, Sign::ToText(Negative) );
                    zgain.show(xml, Sign::ToText(__Zero__) );
                    pgain.show(xml, Sign::ToText(Positive) );
                }
                if(pgain.size)
                {
                    const Gain &winner = **pgain.head;
                    cluster.expand(C0,winner.C);
                }
            } while(pgain.size>0);
            cluster.show(std::cerr, TopLevel, "[", C0, "]", xreal_t::ToString) << std::endl;

            if(klist.size||zgain.size) throw Exception("Equalize: not finished");

        }

#endif

        namespace Conservation
        {



            class Janitor : public Quantized
            {
            public:
                explicit Janitor(const Canon &_);
                virtual ~Janitor() noexcept;

                const Canon &canon;
                XMatrix      cproj;
                XArray       cinit;
                BList        blist;
                XAdd         xadd;

                void fix(XMLog &xml, XWritable &C0, const Level L0);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Janitor);
            };

            Janitor:: Janitor(const Canon &_) :
            canon(_),
            cproj(canon.size,canon.species->size),
            cinit(canon.species->size),
            blist()
            {
            }

            Janitor:: ~Janitor() noexcept
            {

            }

            static inline
            SignType CompareBroken(const BNode * const lhs, const BNode * const rhs) noexcept
            {
                return Sign::Of( (**rhs).xs, (**lhs).xs ); // decreasing
            }

            void Janitor:: fix(XMLog &xml, XWritable &C0, const Level L0)
            {
                Y_XML_SECTION(xml,"Janitor::Fix");
                const xreal_t zero;

                canon.transfer( cinit, AuxLevel, C0, L0);
                if(xml.verbose) canon.show(xml() << "C0=",cinit,xreal_t::ToString) << std::endl;

                Y_XML_COMMENT(xml, "detect broken");
                blist.free();
                for(const LNode *ln=canon.head;ln;ln=ln->next)
                {
                    const Law &   law = **ln;
                    const xreal_t xs  = law.excess(xadd, cinit, AuxLevel);
                    if(xs<=zero) {
                        Y_XMLOG(xml, "(-) " << law.name);
                        continue;
                    }
                    XWritable &cc =  cproj[blist.size+1].ld(cinit);
                    law.project(xadd, cc, cinit, AuxLevel);
                    const Broken broken(law,xs,cc);
                    blist << broken;
                    if( xml.verbose) broken.show( xml() << "(+) ",canon) << std::endl;
                }


                while(blist.size>0)
                {
                    Y_XML_COMMENT(xml, "fixing " << blist.size << "/" << canon.size << " broken");
                    MergeSort::Call(blist,CompareBroken);
                    if(xml.verbose) {
                        for(const BNode *bn=blist.head;bn;bn=bn->next)
                        {
                            (**bn).show(xml(),canon)<<std::endl;
                        }
                    }
                    break;
                }



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

    bool           verbose = true;
    XMLog          xml(verbose);
    Clusters       cls(xml,eqs,0.0);
    Conservation::Wardens wardens(cls);

    std::cerr << "lib=" << lib << std::endl;
    cls.graphViz("cs");


    const size_t m = lib->size();
    XVector      C0(m,0); // concentration

    Library::Concentrations(C0,ran,0.1,0.5);
    lib.show(std::cerr << "C0=", "\t[", C0, "]", xreal_t::ToString ) << std::endl;

    for(const Cluster *cl=cls->head;cl;cl=cl->next)
    {
        for(const Conservation::Canon *canon=cl->canons.head;canon;canon=canon->next)
        {
            Conservation::Janitor janitor(*canon);
            janitor.fix(xml,C0,TopLevel);
        }
    }


    return 0;

    XVector      I0(m,0); // injection
    XVector      C(m,0);

    for(size_t iter=0;iter<100;++iter)
    {
        Library::Concentrations(C0,ran,0.1,0.5);
        lib.show(std::cerr << "C0=", "\t[", C0, "]", xreal_t::ToString ) << std::endl;

        //wardens(xml,C0);

        lib.show(std::cerr << "C0=", "\t[", C0, "]", xreal_t::ToString ) << std::endl;

        for(const Cluster *cl=cls->head;cl;cl=cl->next)
        {
            //Equalizer            eqz(*cl);
            //eqz(xml,C0);
        }
    }


    return 0;


    lib.show(std::cerr << "C0=", "\t[", C0, "]", xreal_t::ToString ) << std::endl;
    lib.show(std::cerr << "I0=", "\t[", I0, "]", xreal_t::ToString ) << std::endl;

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

