
#include "y/chemical/plexus.hpp"

#include "y/sort/heap.hpp"
#include "y/utest/run.hpp"

#include "y/mkl/algebra/rank.hpp"

using namespace Yttrium;
using namespace Chemical;


namespace Yttrium
{
    namespace Chemical
    {

        class Gate : public SSolo
        {
        public:

            //! initialize @empty
            explicit Gate(const size_t capa) :
            SSolo(capa),
            cc()
            {
            }

            //! cleanup
            virtual ~Gate() noexcept
            {

            }

            void ldz() noexcept
            {
                const xreal_t _0;
                free();
                Coerce(cc) = _0;
            }

            void neg() noexcept
            {
                if( __Zero__ != Sign::Of(cc.mantissa) ) Coerce(cc.mantissa) = -cc.mantissa;
            }


            friend std::ostream & operator<<(std::ostream &os, const Gate &self)
            {
                if(self.size<=0)
                {
                    os << "[none]";
                }
                else
                {
                    const SSolo &repo = self;
                    os << real_t(self.cc) << " @" << repo;
                }
                return os;
            }

            //! try to set/update x>=0
            void operator()(const xreal_t  x,
                            const Species &s)
            {
                const xreal_t zero;
                SSolo &       self = *this;
                try {

                    if(size<=0)
                    {
                        // initialize
                        Coerce(cc) =  x;
                        self       << s;
                    }
                    else
                    {
                        switch( Sign::Of(x,cc) )
                        {
                            case Negative: // new winner
                                free();
                                Coerce(cc) =  x;
                                self       << s;
                                break;

                            case __Zero__: // same value
                                self << s;
                                break;

                            case Positive: // discard
                                break;
                        }
                    }
                }
                catch(...)
                {
                    ldz();
                    throw;
                }
            }

            const xreal_t cc;


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Gate);
        };


        class Fixed
        {
        public:
            typedef CxxSeries<Fixed,XMemory> Series;

            //! initialize with no gain
            Fixed(XWritable & _cc, const Conservation::Law &_cl) noexcept :
            gg(),
            cc(_cc),
            cl(_cl)
            {
            }

            //! duplicate
            Fixed(const Fixed &_) noexcept :
            gg(_.gg),
            cc(_.cc),
            cl(_.cl)
            {
            }


            //! cleanup
            ~Fixed() noexcept {}


            bool still(const XReadable &C,
                       const Level      L,
                       XAdd            &xadd)
            {
                return cl.broken(gg,cc,SubLevel,C,L,xadd);
            }

            static int Compare(const Fixed &lhs, const Fixed &rhs) noexcept
            {
                return Comparison::Decreasing(lhs.gg, rhs.gg);
            }

            friend std::ostream & operator<<( std::ostream &os, const Fixed &self)
            {
                os << std::setw(15) << real_t(self.gg) << " @" << self.cl << " -> ";
                self.cl.displayCompact(os,self.cc, SubLevel);
                return os;
            }

            xreal_t                 gg; //!< gain
            XWritable &             cc; //!< persistent fixed concentration
            const Conservation::Law &cl;

        private:
            Y_DISABLE_ASSIGN(Fixed);
        };

        class Warden
        {
        public:

            typedef Conservation::Laws::Group Group;
            typedef Conservation::LNode       LNode;

            explicit Warden(const Cluster &cluster) :
            mine(cluster),
            head( mine.laws.isValid() ? mine.laws->groups.head : 0),
            rows( (0!=head) ? mine.laws->maxGroupSize : 0 ),
            cols( (0!=head) ? mine.species.size       : 0 ),
            xadd( cols ),
            conc( rows, cols),
            jail( rows ),
            cinj( cols ),
            gate( cols )
            {
            }


            void prolog() noexcept
            {
                for(size_t j=cols;j>0;--j) cinj[j].free();
            }

            void run(XWritable &C, const Level L, XMLog &xml)
            {
                Y_XML_SECTION(xml, "Warden");

                for(const Group *g=head;g;g=g->next)
                {
                    const Group                    &G   = *g;
                    const Conservation::Law * const law = wasInjected(G,C,L,xml);
                    if(0!=law)
                    {
                        renormalize(G,C,L,*law,xml);
                    }
                }

                // display if not empty
                if(0!=head)
                {
                    for(const SNode *sn = mine.species.head;sn;sn=sn->next)
                    {
                        const Species &sp = **sn;
                        Y_XMLOG(xml, "d[" << sp << "]=" << cinj[sp.indx[SubLevel]]);
                    }
                }

            }


            virtual ~Warden() noexcept
            {

            }


            const Cluster      &mine;  //!< my cluster
            const Group * const head;  //!< first group
            const size_t        rows;  //!< laws max group size
            const size_t        cols;  //!< max species in sub-level
            XAdd                xadd;  //!< for internal computations
            XMatrix             conc;  //!< workspace for concentrations
            Fixed::Series       jail;  //!< fixed
            XSwell              cinj;  //!< injected
            Gate                gate;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Warden);

            const Conservation::Law * wasInjected(const Group &G,
                                                  XWritable   &C,
                                                  const Level  L,
                                                  XMLog       &xml)
            {

                //--------------------------------------------------------------
                //
                // initialize all possible fixed in group
                //
                //--------------------------------------------------------------
                if(!initialize(G,C,L,xml)) return 0;

                //--------------------------------------------------------------
                //
                // iterative reduction
                //
                //--------------------------------------------------------------
                assert( jail.size() > 0);
                const Conservation::Law *law = 0;
                while(jail.size()>0)
                {
                    law = reduce(C,L,xml); assert(0!=law); // record last reduced
                    if(jail.size()<=0) return law;         // early return
                    update(C,L,xml);
                }

                //--------------------------------------------------------------
                //
                // something was modified, return last reduced
                //
                //--------------------------------------------------------------
                assert(0!=law);
                return law;
            }


            static bool isAdequate(const Equilibrium &eq,
                                   const Gate        &gate)
            {
                const SNode * const node = gate.head;
                if(eq.reac.hiredSome(node) && !eq.prod.hiredSome(node)) return true;
                if(eq.prod.hiredSome(node) && !eq.reac.hiredSome(node)) return true;
                return false;
            }


            void renormalize(const Group             &,
                             XWritable               &C,
                             const Level              L,
                             const Conservation::Law &law,
                             XMLog                   &xml)
            {
                Y_XML_SECTION_OPT(xml, "Renormalize", law);
                Y_XMLOG(xml, "base:" << law.base);

                //--------------------------------------------------------------
                //
                // select most negative remaining species
                //
                //--------------------------------------------------------------
                gate.ldz();
                for(const Actor *a=law->head;a;a=a->next)
                {
                    const Species &sp = a->sp;
                    const xreal_t  cc = C[ sp.indx[L] ];
                    if(cc.mantissa<0) 
                    {
                        Y_XMLOG(xml, "(-) " << std::setw(15) << real_t(cc) << " @[" << sp << "]");
                        gate(cc,sp);
                    }
                    else
                    {
                        Y_XMLOG(xml, "(+) " << std::setw(15) << real_t(cc) << " @[" << sp << "]");
                    }
                }
                Y_XMLOG(xml, " |");
                Y_XMLOG(xml, "(*) " << std::setw(15) << gate);
                Y_XMLOG(xml, " |");



                // select favorable equilibria
                EList eqs;
                for(const ENode *en=law.base.head;en;en=en->next)
                {
                    const Equilibrium &eq = **en;
                    if(!isAdequate(eq,gate)) continue;
                    if(xml.verbose)
                    {
                        mine.display(xml() << "(++) ", eq) << std::endl;
                    }
                    eqs << eq;
                }


                {
                    const size_t m = cols;
                    XArray dC(m);
                    for(const SNode *sn=gate.head;sn;sn=sn->next)
                    {
                        const Species &sp = **sn;
                        dC[ sp.indx[SubLevel] ] = -C[ sp.indx[L] ];
                    }
                    std::cerr << "dC=" << dC << std::endl;

                    const size_t n = eqs.size;
                    Matrix<int> Nu(n,m);
                    {
                        size_t ii = 0;
                        for(const ENode *en=eqs.head;en;en=en->next)
                        {
                            const Equilibrium &eq = **en;
                            eq.topology(Nu[++ii],SubLevel);
                        }
                    }
                    std::cerr << "Nu=" << Nu << std::endl;
                    std::cerr << "rank=" << MKL::Rank::Of(Nu) << std::endl;
                }



                if(gate.size>1)
                {
                    std::cerr << "gates!" << std::endl;
                    exit(1);
                }
            }

            //__________________________________________________________________
            //
            //! load C and compute all modified concentrations
            //__________________________________________________________________
            bool initialize(const Group &group,
                            XWritable   &C,
                            const Level  L,
                            XMLog       &xml)
            {
                Y_XML_SECTION_OPT(xml, "Initialize", "groupSize='" << group.size << "'");
                jail.free();
                bool mustFix = false;
                for(const LNode *ln=group.head;ln;ln=ln->next)
                {
                    const Conservation::Law &cl = **ln;
                    const size_t             ii = jail.size()+1;
                    XWritable               &cc = mine.transfer(conc[ii],SubLevel,C,L);
                    Fixed                    fx(cc,cl);
                    if( fx.still(C,L,xadd) )
                    {
                        jail << fx;
                        Y_XMLOG(xml, "(+) " << fx);
                        mustFix = true;
                    }
                }
                return mustFix;
            }

            //__________________________________________________________________
            //
            //! find lowest fix and apply it
            //__________________________________________________________________
            const Conservation::Law * reduce(XWritable   &C,
                                             const Level  L,
                                             XMLog       &xml)
            {
                Y_XML_SECTION_OPT(xml,"Reduce","size='"<<jail.size()<<"'");

                //------------------------------------------------------
                // sort by decreasing excess
                //------------------------------------------------------
                HeapSort::Call(jail,Fixed::Compare);
                if(xml.verbose)
                {
                    for(size_t i=1;i<jail.size();++i)
                    { Y_XMLOG(xml, "(+) " << jail[i]);     }
                    { Y_XMLOG(xml, "(*) " << jail.tail()); }
                }

                //------------------------------------------------------
                // optimize (and remove) smallest excess
                //------------------------------------------------------
                const Fixed             &fx = jail.tail();
                const Conservation::Law &cl = fx.cl;
                for(const Actor *a=cl->head;a;a=a->next)
                {
                    const Species &      sp = a->sp;
                    const size_t * const id = sp.indx;
                    const size_t         ii = id[SubLevel];
                    const size_t         II = id[L];
                    const xreal_t        c0 = C[ II ];
                    const xreal_t        c1 = fx.cc[ ii ]; assert(c1>=c0);
                    cinj[ii] << (c1-c0);
                    C[II] = c1;
                }
                jail.popTail();
                return &cl;
            }

            //__________________________________________________________________
            //
            //! check who still needs to be fixed
            //__________________________________________________________________
            void update(const XReadable   &C,
                        const Level        L,
                        XMLog             &xml)
            {
                Y_XML_SECTION_OPT(xml,"Update","size='"<<jail.size()<<"'");

                //------------------------------------------------------
                // check remaining with new concentration
                //------------------------------------------------------
                for(size_t i=jail.size();i>0;--i)
                {
                    Fixed &fx = jail[i];
                    if( fx.still(C,L,xadd) )
                    {
                        Y_XMLOG(xml, "(>) " << fx);
                    }
                    else
                    {
                        Y_XMLOG(xml, "(<) " << fx);
                        jail.remove(i);
                    }
                }
            }

        };

    }
}

Y_UTEST(plexus)
{

    Plexus plexus(true);
    const Library    &lib = plexus.lib;
    const Equilibria &eqs = plexus.eqs;
    XMLog            &xml = plexus.xml;

#if 0
    plexus("@water @oxalic.*");

    std::cerr << "lib=" <<  lib << std::endl;
    std::cerr << "eqs=" <<  eqs << std::endl;
    Clusters &cls = plexus.assemble();


    XVector C0(lib->size(),0);
    const Species &ah2 = lib["OxH2"];
    const Species &ahm = lib["OxH-"];
    const Species &amm = lib["Ox--"];

    C0[ ah2.indx[TopLevel] ] =  1.0;
    C0[ ahm.indx[TopLevel] ] = -0.7;
    C0[ amm.indx[TopLevel] ] = -0.7;

    for(const Cluster *cl=cls->head;cl;cl=cl->next)
    {

        Warden warden(*cl);
        lib(std::cerr << "C0=","\t[",C0,"]");
        warden.run(C0,TopLevel,xml);
        lib(std::cerr << "C1=","\t[",C0,"]");
    }

#else
    for(int i=1;i<argc;++i)
    {
        plexus(Jive::Module::OpenData(argv[i],argv[i]));
    }

    std::cerr << "lib=" << lib << std::endl;
    std::cerr << "eqs=" << eqs << std::endl;

    Clusters        &cls = plexus.assemble();
    //const XReadable &K   = cls.K(0);


    XVector C0(lib->size(),0);
    XVector dC(C0);


    for(const Cluster *cl=cls->head;cl;cl=cl->next)
    {

        Warden warden(*cl);
        for(size_t iter=0;iter<1;++iter)
        {
            plexus.conc(C0,0.3,0.5);
            warden.prolog();
            lib(std::cerr << "C0=","\t[",C0,"]");
            warden.run(C0,TopLevel,xml);
            lib(std::cerr << "C1=","\t[",C0,"]");
        }


    }
#endif

}
Y_UDONE()

