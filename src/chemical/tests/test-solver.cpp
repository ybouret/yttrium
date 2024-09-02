
#include "y/chemical/plexus.hpp"
#include "y/chemical/plexus/wardens.hpp"

#include "y/chemical/plexus/joint.hpp"
#include "y/chemical/reactive/aftermath.hpp"
#include "y/sort/heap.hpp"

#include "y/orthogonal/family.hpp"

#include "y/utest/run.hpp"



namespace Yttrium
{
    namespace Chemical
    {

        class Prospect
        {
        public:
            typedef CxxSeries<Prospect> Series;

            explicit Prospect(const Situation     _st,
                              const Equilibrium & _eq,
                              const xreal_t       _ek,
                              const XReadable &   _cc,
                              const xreal_t       _xi) noexcept:
            st(_st),
            eq(_eq),
            ek(_ek),
            cc(_cc),
            xi(_xi),
            ax(xi.abs())
            {
            }

            Prospect(const Prospect &_) noexcept :
            st(_.st),
            eq(_.eq),
            ek(_.ek),
            cc(_.cc),
            xi(_.xi),
            ax(_.ax)
            {
            }

            ~Prospect() noexcept {}

            std::ostream & show(std::ostream &os, const Cluster &cl, const XReadable &Ktop) const
            {
                os << std::setw(15) << real_t(xi) << " @";
                cl.display(os,eq,Ktop);
                return os;
            }

            static int Compare(const Prospect &lhs, const Prospect &rhs) noexcept
            {
                return Comparison::Decreasing(lhs.ax, rhs.ax);
            }

            const Situation     st;
            const Equilibrium & eq;
            const xreal_t       ek;
            const XReadable &   cc;
            const xreal_t       xi;
            const xreal_t       ax;

        private:
            Y_DISABLE_ASSIGN(Prospect);
        };

        class Solver : public Joint
        {
        public:

            explicit Solver(const Cluster &cl) :
            Joint(cl),
            afm(),
            ceq(neqs,nspc),
            pps(neqs),
            dof(mine.Nu.rows),
            ortho(nspc,dof),
            ebank(),
            basis(ebank)
            {
            }

            virtual ~Solver() noexcept {}


            void process(XWritable &C, const Level L, const XReadable &Ktop, XMLog &xml);

            Aftermath          afm;
            XMatrix            ceq;
            Prospect::Series   pps;
            const size_t       dof;
            Orthogonal::Family ortho;
            EBank              ebank;
            ERepo              basis;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Solver);

            void showProspects(XMLog &xml, const XReadable &Ktop) const
            {
                if(xml.verbose)
                {
                    for(size_t i=1;i<=pps.size();++i)
                    {
                        pps[i].show( xml(), mine, Ktop) << std::endl;
                    }
                }
            }



        };

        void Solver:: process(XWritable &       C,
                              const Level       L,
                              const XReadable & Ktop,
                              XMLog           & xml)
        {

            Y_XML_SECTION(xml, "process");

            size_t cycle = 0;
            ortho.free();
            basis.free();
        PROSPECT:
            {
                ++cycle;
                Y_XMLOG(xml, "[cycle #" << cycle << "]");
                pps.free();
                bool emergency = false; // keep only crucial otherwise

                for(const ENode *en=mine.head;en;en=en->next)
                {
                    const Equilibrium &eq = **en;
                    const xreal_t      ek = Ktop[eq.indx[TopLevel]];
                    const size_t       ii = pps.size() + 1;
                    XWritable         &cc = mine.transfer(ceq[ii],SubLevel,C,L);
                    const Situation    st = afm.seek(cc, SubLevel, eq, ek);

                    switch(st)
                    {
                        case Blocked:
                            continue;

                        case Running:
                            if(emergency) continue;
                            Y_XMLOG(xml, "[Running] " << eq);
                            break;

                        case Crucial:
                            if(!emergency)
                            {
                                emergency = true;
                            }
                            Y_XMLOG(xml, "[Crucial] " << eq);
                            break;
                    }

                    const Prospect pro(st,eq,ek,cc,afm.eval(cc,SubLevel, C, L, eq) );
                    pps << pro;
                }

                if(pps.size() <= 0 )
                {
                    Y_XMLOG(xml, "[Jammed]");
                    return;
                }


                if(emergency)
                {
                    for(size_t i=pps.size();i>0;--i) {
                        if( Crucial != pps[i].st ) {
                            assert(Running==pps[i].st);
                            pps.remove(i);
                        }
                    }

                    Y_XML_SECTION_OPT(xml, "emergency","count='"<<pps.size() << "'");

                    assert( pps.size() > 0);

                    HeapSort::Call(pps,Prospect::Compare);
                    showProspects(xml,Ktop);

                    const Prospect &pro = pps.head();
                    mine.transfer(C, L, pro.cc, SubLevel);
                    goto PROSPECT;

                }
            }

            //
            Y_XML_SECTION_OPT(xml, "running", "count='" << pps.size() << "'");
            HeapSort::Call(pps,Prospect::Compare);
            showProspects(xml,Ktop);
            


            // select family
            for(size_t i=1;i<=pps.size();++i)
            {
                const Prospect    &   pro = pps[i];
                const Equilibrium &   eq  = pro.eq;
                const size_t          ei  = eq.indx[SubLevel];
                const Readable<int> & nu  = mine.iTopo[ei];
                if(ortho.wouldAccept(nu))
                {
                    ortho.expand();
                    basis << eq;
                    if(ortho.size>=dof) break;
                }
            }

            Y_XMLOG(xml, "family=" << basis);



        }

    }
}

using namespace Yttrium;
using namespace Chemical;


Y_UTEST(solver)
{

    Plexus plexus(true);

    for(int i=1;i<argc;++i)
    {
        plexus(Jive::Module::OpenData(argv[i],argv[i]));
    }

    Clusters        &cls = plexus.assemble();
    const XReadable &K   = cls.K(0);
    Wardens          ward(cls);

    const Library    &lib = plexus.lib;
    //const Equilibria &eqs = plexus.eqs;
    XMLog            &xml = plexus.xml;

    XVector C0(lib->size(),0);
    XVector dC(C0);

    for(const Cluster *cl=cls->head;cl;cl=cl->next)
    {
        Solver solver(*cl);

        for(size_t iter=0;iter<1;++iter)
        {
            plexus.conc(C0,0.3,0.5);
            lib(std::cerr << "C0=","\t[",C0,"]");
            ward(C0,dC,TopLevel,xml);
            lib(std::cerr << "C0=","\t[",C0,"]");

            solver.process(C0, TopLevel, K, xml);

        }
    }




}
Y_UDONE()
