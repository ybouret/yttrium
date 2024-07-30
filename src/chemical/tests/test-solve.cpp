
#include "y/chemical/plexus/clusters.hpp"
#include "y/chemical/weasel/compiler.hpp"
#include "y/random/park-miller.hpp"
#include "y/utest/run.hpp"

#include "y/container/cxx/series.hpp"
#include "y/chemical/reactive/aftermath.hpp"
#include "y/sort/heap.hpp"
#include "y/system/exception.hpp"
#include "y/orthogonal/family.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        class Applicant
        {
        public:
            typedef CxxSeries<Applicant> Series;

            Applicant(const Equilibrium &  _eq,
                      const xreal_t        _eK,
                      const XReadable    & _cc,
                      const xreal_t        _xi) noexcept :
            eq(_eq),
            eK(_eK),
            cc(_cc),
            xi(_xi),
            ax( xi.abs() )
            {
            }

            Applicant(const Applicant &_) noexcept :
            eq(_.eq),
            eK(_.eK),
            cc(_.cc),
            xi(_.xi),
            ax(_.ax)
            {
            }

            size_t sub() const noexcept { return eq.indx[SubLevel]; }

            real_t affinity(XMul            &X,
                            const XReadable &C,
                            const Level      L) const
            {
                return eq.affinity(eK, X, C, L);
            }


            static int Compare(const Applicant &lhs, const Applicant &rhs) noexcept
            {
                return Comparison::Increasing<xreal_t>(lhs.ax,rhs.ax);
            }


            std::ostream & display(std::ostream   &os,
                                   const Assembly &uuid,
                                   const bool      full) const
            {
                uuid.pad(os<< eq, eq);
                os << " | ax =" << std::setw(15) << real_t(ax);
                if(full)
                {
                    eq.displayCompact(os << " @",cc, SubLevel);
                }
                return os;
            }

            const Equilibrium &eq;
            const xreal_t      eK;
            const XReadable   &cc;
            const xreal_t      xi;
            const xreal_t      ax; //!< |xi|

        private:
            Y_DISABLE_ASSIGN(Applicant);
        };

        typedef Small::CoopLightList<const Applicant> AList;
        typedef AList::NodeType                       ANode;
        typedef AList::ProxyType                      ABank;


        class Normalizer : public Counted
        {
        public:
            explicit Normalizer(const Cluster &cl) :
            ceq(cl.size,cl.species.size),
            aps(cl.size),
            obj(cl.size),
            dof(cl.Nu.rows),
            bnk(),
            apl(bnk),
            qfm(cl.species.size,cl.size)
            {
            }

            virtual ~Normalizer() noexcept
            {
            }


            void run(const Cluster   & cl,
                     XWritable       & Ctop,
                     const XReadable &Ktop,
                     XMLog           & xml)
            {
                assert(cl.size<=ceq.rows);
                assert(cl.species.size==ceq.cols);
                Y_XML_SECTION_OPT(xml, "Normalizer ", " size='" << cl.size << "' species='" << cl.species.size << "'");
                bool         repl = false;
                const size_t nmax = examine(cl, Ctop, Ktop, repl, xml); if(nmax<=0) { Y_XMLOG(xml, "[Jammed!]"); return; }
                const size_t base = extract(cl,xml);

                switch(base)
                {
                    case 1:
                        break;
                    default:
                        break;
                }

                if(!repl)
                    Y_XMLOG(xml, std::setw(15) << real_t(objectiveFunction(Ctop, TopLevel)) << " @Ctop");
                else
                    Y_XMLOG(xml, "[replaced Ctop]");

                for(const ANode *an=apl.head;an;an=an->next)
                {
                    const Applicant &app = **an;
                    Y_XMLOG(xml,std::setw(15) << real_t(objectiveFunction(app.cc,SubLevel)) << " @" << app.eq);
                }

            }

            xreal_t objectiveFunction(const XReadable &C, const Level L)
            {
                obj.free();
                for(const ANode *an=apl.head;an;an=an->next)
                {
                    obj << (**an).affinity(afm.xmul, C, L);
                }
                return afm.xadd.normOf(obj);
            }

            Aftermath          afm;
            XMatrix            ceq;
            Applicant::Series  aps;
            CxxSeries<xreal_t> obj;
            const size_t       dof; //!< primary eqs
            ABank              bnk;
            AList              apl;
            Orthogonal::Family qfm;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Normalizer);

            size_t   extract(const Cluster &cl, XMLog &xml)
            {
                Y_XML_SECTION(xml, "ExtractFamily");

                apl.free();
                qfm.free();

                const size_t nap = aps.size();
                for(size_t i=1;i<=nap;++i)
                {
                    const Applicant &app = aps[i];
                    if( qfm.wouldAccept( cl.topology[ app.sub() ] ) )
                    {
                        apl << app;
                        qfm.expand();
                        if( apl.size >= dof) break;
                    }
                }

                Y_XMLOG(xml, "#applicant  = " << aps.size());
                Y_XMLOG(xml, "#primary    = " << dof);
                Y_XMLOG(xml, "#family     = " << apl.size);
                if(xml.verbose)
                {
                    for(const ANode *an=apl.head;an;an=an->next)
                    {
                        const Applicant &app = **an;
                        app.display( xml() << "| ", cl.uuid, false) << std::endl;
                    }
                }
                return apl.size;
            }

            size_t examine(const Cluster   & cl,
                           XWritable       & Ctop,
                           const XReadable & Ktop,
                           bool            & repl,
                           XMLog           &  xml)
            {
                Y_XML_SECTION(xml, "QueryApplicants");
                repl        = false;
                size_t iter = 0;
            EXAMINE:
                ++iter;
                Y_XMLOG(xml, "[Examine] [#iter=" << iter << "]");
                aps.free();
                for(const ENode *en=cl.head;en;en=en->next)
                {
                    const Equilibrium &eq = **en;
                    const xreal_t      eK = Ktop[ eq.indx[TopLevel] ];
                    const size_t       ii = aps.size()+1;
                    XWritable         &cc = cl.transfer(ceq[ii], SubLevel, Ctop, TopLevel);
                    const Situation    st = afm.seek(cc, SubLevel, eq, eK);

                    switch(st)
                    {
                        case Blocked:
                            Y_XMLOG(xml, "[Blocked] " << eq.name);
                            continue;

                        case Crucial:
                            Y_XMLOG(xml, "[Crucial] " << eq.name);
                            cl.transfer(Ctop, TopLevel, cc, SubLevel);
                            repl = true;
                            goto EXAMINE;

                        case Running:
                            Y_XMLOG(xml, "[Running] " << eq.name);
                            break;
                    }

                    assert(Running==st);
                    const Applicant app(eq,eK,cc, afm.eval(cc, SubLevel, Ctop, TopLevel, eq) );
                    aps << app;
                }


                HeapSort::Call(aps, Applicant::Compare);
                const size_t nmax = aps.size();
                for(size_t i=1;i<=nmax;++i)
                {
                    const Applicant   &lhs = aps[i]; if(xml.verbose)  lhs.display( xml() << "| ", cl.uuid, true) << std::endl;
                    const Equilibrium &lEq = lhs.eq;
                    const char * const lid = lEq.name.c_str();

                    if( !lEq.reac.accounted(Ctop, TopLevel) ) throw Specific::Exception(lid, "missing reactant(s) on top-level");
                    if( !lEq.prod.accounted(Ctop, TopLevel) ) throw Specific::Exception(lid, "missing product(s) on top-level");

                    for(size_t j=1;j<=nmax;++j)
                    {
                        const Applicant   &rhs = aps[j];
                        const char * const rid = rhs.eq.name.c_str();
                        if(!lEq.reac.accounted(rhs.cc,SubLevel)) throw Specific::Exception(lid, "missing reactant(s) in '%s'", rid);
                        if(!lEq.prod.accounted(rhs.cc,SubLevel)) throw Specific::Exception(lid, "missing reactant(s) in '%s'", rid);
                    }
                }

                return nmax;
            }

        };

    }
}



using namespace Yttrium;
using namespace Chemical;





Y_UTEST(solve)
{

    Weasel::Compiler  &weasel = Weasel::Compiler::Instance();
    Library            lib;
    LuaEquilibria      eqs;
    Random::ParkMiller ran;

    for(int i=1;i<argc;++i)
    {
        weasel(lib,eqs,Jive::Module::OpenData(argv[i],argv[i]));
    }

    std::cerr << "lib=" << lib << std::endl;
    std::cerr << "eqs=" << eqs << std::endl;

    bool     verbose = true;
    XMLog    xml(verbose);
    Clusters clusters(eqs,xml);
    const XReadable &K = clusters.K(0);

    XVector C0(lib->size(),0);

    for(size_t iter=0;iter<1;++iter)
    {
        Species::Conc(C0,ran,0.3);

        //C0.ld(0);

        lib(std::cerr << "C0=","\t[",C0,"]");

        for(const Cluster *cl=clusters->head;cl;cl=cl->next)
        {
            Normalizer normalize(*cl);

            normalize.run(*cl, C0, K, xml);
        }



    }



}
Y_UDONE()
