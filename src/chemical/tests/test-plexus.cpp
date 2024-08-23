
#include "y/chemical/plexus.hpp"

#include "y/sort/heap.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;
using namespace Chemical;


namespace Yttrium
{
    namespace Chemical
    {

        class Gate : public SRepo
        {
        public:

            //! initialize @empty
            explicit Gate(const SBank &sb) noexcept :
            SRepo(sb)
            {
            }

            //! cleanup
            virtual ~Gate() noexcept
            {

            }

            void neg() noexcept
            {
                if( __Zero__ != Sign::Of(xi.mantissa) ) Coerce(xi.mantissa) = -xi.mantissa;
            }


            friend std::ostream & operator<<(std::ostream &os, const Gate &self)
            {
                if(self.size<=0)
                {
                    os << "[none]";
                }
                else
                {
                    const SRepo &repo = self;
                    os << real_t(self.xi) << " @" << repo;
                }
                return os;
            }

            //! try to set/update x>=0
            void operator()(const xreal_t x,
                            const Species &s)
            {
                const xreal_t zero;
                SRepo &       self = *this;
                try {

                    if(size<=0)
                    {
                        // initialize
                        Coerce(xi) =  x;
                        self       << s;
                    }
                    else
                    {
                        switch( Sign::Of(x,xi) )
                        {
                            case Negative: // new winner
                                free();
                                Coerce(xi) =  x;
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
                    free();
                    Coerce(xi) = zero;
                    throw;
                }
            }

            const xreal_t xi;


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
            cinj( cols )
            {
            }


            void prolog() noexcept
            {
                for(size_t j=rows;j>0;--j) cinj[j].free();
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
                    law = reduce(C,L,xml); assert(0!=law);
                    if(jail.size()<=0) return law; // early return, needed fixed
                    update(C,L,xml);
                }

                //--------------------------------------------------------------
                //
                // something was modified
                //
                //--------------------------------------------------------------
                assert(0!=law);
                return law;
            }

            void renormalize(const Group             &G,
                             XWritable               &C,
                             const Level              L,
                             const Conservation::Law &law,
                             XMLog                   &xml)
            {
                Y_XML_SECTION_OPT(xml, "Renormalize", law);
                Y_XMLOG(xml, "from:" << G);
                Y_XMLOG(xml, "base:" << G.base);
                Y_XMLOG(xml, "crew:" << G.crew);

                // collect most negative species
                SBank sb;
                Gate  gate(sb);
                {
                    Y_XML_SECTION(xml, "findGate");
                    for(const SNode *sn=G.crew.head;sn;sn=sn->next)
                    {
                        // check species
                        const Species &sp = **sn; assert(mine.conserved.book.has(sp));
                        const xreal_t  cc = C[ sp.indx[L] ];
                        if(cc.mantissa>=0)
                        {
                            Y_XMLOG(xml, "(+) " << std::setw(15) << real_t(cc) << " =[" << sp << "]");
                            continue;
                        }
                        else
                        {
                            Y_XMLOG(xml, "(+) " << std::setw(15) << real_t(cc) << " =[" << sp << "]");
                            gate(cc,sp);
                        }
                    }

                    assert(gate.size>0);

                    gate.neg();
                    Y_XMLOG(xml, "(*) " << std::setw(15) << gate << " to find...") ;

                }
                {
                    Y_XML_SECTION(xml, "findPlan");

                    // finding a way
                    for(const ENode *en=G.base.head;en;en=en->next)
                    {
                        const Equilibrium &eq = **en;
                        for(const SNode *sn=gate.head;sn;sn=sn->next)
                        {
                            const Species &         sp = **sn;
                            const Component * const cm = eq.query(sp);
                            if(!cm) continue;
                            Y_XMLOG(xml, "(+) " << eq << " with component '" << *cm << "'");
                        }
                    }

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
                bool ans = false;
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
                        ans = true;
                    }
                }
                return ans;
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
                const Conservation::Law *cl = &fx.cl;
                for(const Actor *a=(*cl)->head;a;a=a->next)
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
                return cl;
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

    for(int i=1;i<argc;++i)
    {
        plexus(Jive::Module::OpenData(argv[i],argv[i]));
    }

    std::cerr << "lib=" << plexus.lib << std::endl;
    std::cerr << "eqs=" << plexus.eqs << std::endl;

    const Library   &lib = plexus.lib;
    Clusters        &cls = plexus.assemble();
    XMLog           &xml = plexus.xml;
    //const XReadable &K   = cls.K(0);


    XVector C0(lib->size(),0);
    XVector dC(C0);


    for(const Cluster *cl=cls->head;cl;cl=cl->next)
    {
        Warden warden(*cl);
        plexus.conc(C0,0.3,0.5);
        lib(std::cerr << "C0=","\t[",C0,"]");
        warden.run(C0,TopLevel,xml);
        lib(std::cerr << "C1=","\t[",C0,"]");


    }

}
Y_UDONE()

