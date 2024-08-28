
#include "y/chemical/plexus.hpp"

#include "y/sort/heap.hpp"
#include "y/utest/run.hpp"


#include "y/data/small/heavy/list/coop.hpp"
#include "y/system/exception.hpp"

using namespace Yttrium;
using namespace Chemical;


namespace Yttrium
{
    namespace Chemical
    {


        //! proxy to an internal dynamic repository of species
        class SProxy : public Proxy<const SRepo>
        {
        public:
            explicit SProxy(const SBank &sbank) noexcept : Proxy<const SRepo>(), sr(sbank) {}
            virtual ~SProxy() noexcept {}
            explicit SProxy(const SProxy &_) : Proxy<const SRepo>(), sr(_.sr) {}

            SProxy & operator<<(const SProxy &other)
            {
                sr << other.sr;
                return *this;
            }

        protected:
            SRepo sr;

        private:
            Y_DISABLE_ASSIGN(SProxy);
            virtual ConstInterface & surrogate() const noexcept { return sr; }
        };

    }
}



namespace Yttrium
{
    namespace Chemical
    {


        //! single frontier to find limiting extent to a transformation
        class SingleFrontier : public SProxy, public Recyclable
        {
        public:
            explicit SingleFrontier(const SBank &sbank) noexcept :
            SProxy(sbank),
            xi()
            {
            }

            virtual ~SingleFrontier() noexcept { }


            // Methods
            virtual void  free() noexcept
            {
                sr.free();
                Coerce(xi).ldz();
            }

            friend std::ostream & operator<<(std::ostream &os, const SingleFrontier &self)
            {
                os << "lim=";
                if(self->size<=0)
                    os << "none";
                else
                    os << real_t(self.xi) << "@" << self.sr;
                return os;
            }

            // initialize/update with x>=0
            void operator()(const xreal_t  x,
                            const Species &s)
            {
                assert(x>=0.0);
                try {
                    if(sr.size<=0)
                    {
                        first(x,s);
                    }
                    else
                    {
                        switch( Sign::Of(x,xi) )
                        {
                            case Negative: // new smallest
                                free();
                                first(x,s);
                                break;

                            case __Zero__: // same value
                                sr << s;
                                break;


                            case Positive: // discard
                                break;

                        }
                    }
                }
                catch(...)
                {
                    free();
                    throw;
                }
            }

            bool blocking() const noexcept
            {
                assert(xi.mantissa>=0);
                return sr.size > 0 && xi.mantissa <= 0;
            }


            const xreal_t xi;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(SingleFrontier);
            void first(const xreal_t x, const Species &s)
            {
                assert(0==sr.size);
                sr << s;
                Coerce(xi) = x;
            }
        };

        //! fixed value frontier, with one or more species
        class Frontier : public SProxy
        {
        public:

            explicit Frontier(const SBank &sbank,
                              const xreal_t  x,
                              const Species &s) :
            SProxy(sbank),
            xi(x)
            {
                sr << s;
            }

            virtual ~Frontier() noexcept
            {
            }

            explicit Frontier(const Frontier &F) :
            SProxy(F),
            xi(F.xi)
            {
            }

            friend std::ostream & operator<<(std::ostream &os, const Frontier &self)
            {
                assert(self.sr.size>0);
                os << real_t(self.xi) << "@" << self.sr;
                return os;
            }

            //! wrapper
            Frontier & operator<<(const Species &s)
            {
                sr << s;
                return *this;
            }

            const xreal_t xi;
        private:
            Y_DISABLE_ASSIGN(Frontier);
        };

        typedef Small::CoopHeavyList<Frontier> FList;
        typedef FList::ProxyType               FBank;
        typedef FList::NodeType                FNode;

        class Fund
        {
        public:
            explicit Fund() : sbank(), lbank(), fbank()
            {
            }

            SBank               sbank;
            Conservation::LBank lbank;
            FBank               fbank;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Fund);
        };


        //! frontiers to sort multiple requirements
        class Frontiers : public FList
        {
        public:
            explicit Frontiers(const Fund &fund) noexcept :
            FList(fund.fbank),
            sbank(fund.sbank)
            {
            }

            virtual ~Frontiers() noexcept
            {
            }

            bool sorted() const noexcept
            {
                if(size<=1) return true;

                for(const FNode *node=head,*next=head->next;0!=next;node=next,next=node->next)
                {
                    if( (**node).xi >= (**next).xi ) return false;
                }

                return true;
            }

            void operator()(const xreal_t  xi,
                            const Species &sp)
            {
                switch(size)
                {
                    case 0: pushTail( make(xi,sp) ); return;
                    case 1: {
                        Frontier &f = **head;
                        switch( Sign::Of(xi, f.xi) )
                        {
                            case Negative: pushHead( make(xi,sp) ); break;
                            case __Zero__: f << sp;                 break;
                            case Positive: pushTail( make(xi,sp) ); break;
                        }
                        assert(sorted());
                    } return;

                    default:
                        break;
                }
                assert(size>=2);

                FNode * lower = head;
                switch( Sign::Of(xi, (**lower).xi) )
                {
                    case Negative: pushHead( make(xi,sp) ); assert(sorted()); return;
                    case __Zero__: **lower << sp;           assert(sorted()); return;
                    case Positive: break;
                }

                FNode * const upper = tail;
                switch( Sign::Of(xi, (**upper).xi) )
                {
                    case Negative: break;
                    case __Zero__: **upper << sp;            assert(sorted()); return;
                    case Positive: pushTail( make(xi,sp) );  assert(sorted()); return;
                }

            CYCLE:
                FNode *probe = lower->next;

                if(upper!=probe)
                {
                    switch( Sign::Of(xi,(**probe).xi) )
                    {
                        case Negative:
                            break; // after lower
                        case __Zero__: (**probe) << sp;  assert(sorted()); return;
                        case Positive:
                            lower = probe;
                            goto CYCLE;
                    }
                }

                insertAfter(lower, make(xi,sp) );
                assert(sorted());


            }

            friend std::ostream & operator<<(std::ostream &os, const Frontiers &self)
            {
                const FList &F = self;
                os << "req=";
                if(F.size<=0)
                    os << "none";
                else
                    os << F;
                return os;
            }

        private:
            const SBank sbank;
            Y_DISABLE_COPY_AND_ASSIGN(Frontiers);

            FNode *make(const xreal_t  xi,
                        const Species &sp)
            {
                const Frontier  f(sbank,xi,sp);
                return proxy->produce(f);
            }

        };

        class Trim : public Recyclable
        {
        public:
            explicit Trim(const Fund &fund) noexcept :
            limiting(fund.sbank),
            required(fund)
            {
            }

            virtual ~Trim() noexcept
            {
            }

            virtual void free() noexcept
            {
                limiting.free();
                required.free();
            }

            bool operator()(const XReadable   &C,
                            const Level        L,
                            const Actors      &A,
                            const AddressBook &conserved)
            {
                free();
                try
                {
                    for(const Actor *a=A->head;a;a=a->next)
                    {
                        const Species &sp = a->sp; if(!conserved.has(sp)) continue;
                        const xreal_t  cc = C[ sp.indx[L] ];
                        if(cc.mantissa>=0)
                        {
                            limiting(cc/a->xn,sp);
                        }
                        else
                        {
                            required(-cc/a->xn,sp);
                            assert(required.sorted());
                        }
                    }
                    return required.size>0;
                }
                catch(...)
                {
                    free();
                    throw;
                }
            }

            friend std::ostream & operator<<(std::ostream &os, const Trim &self)
            {
                os << self.limiting << "/" << self.required;
                return os;
            }

            SingleFrontier  limiting; //!< from positive of zero concentrations
            Frontiers       required; //!< from negative concentrations

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Trim);
        };





        //! Trims for reactants and products
        class Trims : public Recyclable
        {
        public:
            typedef CxxArray<Trims,XMemory> Array;
            enum Kind
            {
                BadNone,
                BadReac,
                BadProd,
                BadBoth
            };

            explicit Trims(const Fund &fund) noexcept :
            reac(fund),
            prod(fund)
            {
            }

            virtual ~Trims() noexcept
            {
            }

            virtual void free() noexcept
            {
                reac.free();
                prod.free();
            }

            Kind operator()(const XReadable   &C,
                            const Level        L,
                            const Components  &E,
                            const AddressBook &conserved)
            {
                free();
                try {
                    if(reac(C,L,E.reac,conserved))
                    {
                        assert(reac.required.size>0);
                        if(prod(C,L,E.prod,conserved))
                        {
                            assert(reac.required.size>0);
                            assert(prod.required.size>0);
                            return BadBoth;
                        }
                        else
                        {
                            assert(reac.required.size>0);
                            assert(prod.required.size<=0);
                            return BadReac;
                        }

                    }
                    else
                    {
                        assert(reac.required.size<=0);
                        if(prod(C,L,E.prod,conserved))
                        {
                            assert(reac.required.size<=0);
                            assert(prod.required.size>0);
                            return BadProd;
                        }
                        else
                        {
                            assert(reac.required.size<=0);
                            assert(prod.required.size<=0);
                            return BadNone;
                        }
                    }
                } catch(...) {
                    free(); throw;
                }
            }


            Trim           reac;
            Trim           prod;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Trims);
        };



        //! Fixed concentrations
        class Fixed
        {
        public:
            typedef CxxSeries<Fixed,XMemory> Series;

            //! initialize with no gain
            Fixed(XWritable &              _cc,
                  const Conservation::Law &_cl) noexcept :
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


        class Trade
        {
        public:
            typedef CxxSeries<Trade,XMemory> Series;

            Trade(const Equilibrium & _eq,
                  const XReadable &   _cc,
                  const xreal_t       _gg,
                  const XReadable &   _dc) noexcept :
            eq(_eq),
            cc(_cc),
            gg(_gg),
            dc(_dc)
            {}

            ~Trade() noexcept
            {
            }

            Trade(const Trade &_) noexcept :
            eq(_.eq),
            cc(_.cc),
            gg(_.gg),
            dc(_.dc)
            {
            }

            static int Compare(const Trade &lhs, const Trade &rhs) noexcept
            {
                return Comparison::Decreasing(lhs.gg,rhs.gg);
            }

            friend std::ostream & operator<<( std::ostream &os, const Trade &tr)
            {
                os << std::setw(15) << real_t(tr.gg) << " @" << tr.eq;
                tr.eq.displayCompact(os << ' ', tr.cc, SubLevel);
                return os;
            }


            const Equilibrium & eq;
            const XReadable   & cc;
            const xreal_t       gg;
            const XReadable   & dc;

        private:
            Y_DISABLE_ASSIGN(Trade);
        };


        class Warden : public Quantized
        {
        public:

            typedef Conservation::Laws::Group Group;
            typedef Conservation::LNode       LNode;
            typedef Conservation::LRepo       LRepo;

            explicit Warden(const Cluster &cluster) :
            mine(cluster),
            head( mine.laws.isValid() ? mine.laws->groups.head : 0),
            rows( (0!=head) ? mine.laws->maxGroupSize : 0 ),
            cols( (0!=head) ? mine.species.size       : 0 ),
            xadd( cols ),
            conc( rows, cols),
            jail( rows ),
            cinj( cols ),
            fund(),
            lawz(fund.lbank),
            trms(fund),
            best(fund.sbank),
            wobbly(fund.sbank),
            ctrade(mine.size,mine.species.size),
            dtrade(mine.size,mine.species.size),
            trades(mine.size),
            xaccum(mine.species.size)
            {
            }


            void prolog() noexcept
            {
                cinj.forEach( & XAdd::free );
            }


            bool sanitize(XWritable &C, const Level L, XMLog &xml)
            {

                Y_XML_SECTION(xml, "sanitizing" );

                // inject corrections and detect zero laws
                lawz.free();
                for(const Group *g=head;g;g=g->next)
                {
                    const Group                    &G   = *g;
                    const Conservation::Law * const law = wasInjected(G,C,L,xml);
                    if(0!=law)
                    {
                        lawz << *law;
                    }
                }


                // optional output
                if(0!=head)
                {
                    Y_XMLOG(xml, "lawz=" << lawz);

                    for(const SNode *sn = mine.species.head;sn;sn=sn->next)
                    {
                        const Species &sp = **sn;
                        Y_XMLOG(xml, "d[" << sp << "]=" << cinj[sp.indx[SubLevel]]);
                    }
                }

                for(const SNode *sn=mine.species.head;sn;sn=sn->next)
                {
                    if( C[ (**sn).indx[L]].mantissa < 0 ) return true;
                }

                return false;
            }


            void equalize(XWritable &C, const Level L, XMLog &xml)
            {
                if(0==head) return;

                Y_XML_SECTION(xml, "equalizing" );

                //--------------------------------------------------------------
                //
                // process limited equilibria
                //
                //--------------------------------------------------------------
                size_t cycle = 0;
            CYCLE:
                ++cycle;
                Y_XMLOG(xml, "-------- #cycle = " << cycle << " --------");
                const size_t unbalanced = getUnbalanced(C, L, xml);
                Y_XMLOG(xml, "(#) unbalanced = " << unbalanced);
                if(unbalanced<=0)
                {
                    assert(0==trades.size());
                    assert(0==wobbly.size);
                    for(const LNode *ln=lawz.head;ln;ln=ln->next)
                    {
                        const Conservation::Law &law = **ln;
                        Y_XMLOG(xml, "check " << law);
                    }
                    return;
                }

                const size_t tradeCount = trades.size();
                Y_XMLOG(xml, "(#) tradeCount = " << tradeCount << " / " << wobbly);
                for(const SNode *sn=wobbly.head;sn;sn=sn->next)
                {
                    const Species &sp = **sn;
                    Y_XMLOG(xml, std::setw(15) << real_t( C[sp.indx[L]]) << " = [" << sp << "]");
                }

                if(tradeCount<=0)
                {
                    std::cerr << "Not Trade!!" << std::endl;

                    std::cerr << "Lawz = " << lawz << std::endl;
                    exit(9);
                    return;
                }


                optimizeTrade(C, L, xml);
                goto CYCLE;


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
            Fund                fund;
            LRepo               lawz;  //!< laws with zero values
            Trims               trms;
            SingleFrontier      best;    //!< best effort to equalize
            SRepo               wobbly;  //!< negative species list
            XMatrix             ctrade;  //!< traded concentrations
            XMatrix             dtrade;  //!< traded changes
            Trade::Series       trades;  //!< trades
            XSwell              xaccum;  //!< accumulators for multiple trades

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Warden);

            void   optimizeTrade(XWritable &C, const Level L, XMLog &xml)
            {

                Y_XML_SECTION(xml,"optimizeTrade");
                HeapSort::Call(trades,Trade::Compare);
                const Trade &top = trades.head();

                //--------------------------------------------------------------
                //
                // look for successive fully detached trades
                //
                //--------------------------------------------------------------
                {
                    Y_XML_SECTION_OPT(xml, "content", "size='" << trades.size() << "'");

                    Y_XMLOG(xml, "(*) " << top);

                    // keep first trade, look for detached
                    for(size_t i=2;i<=trades.size();)
                    {
                        const Trade          &tri      = trades[i];
                        const Equilibrium    &lhs      = tri.eq;
                        const Readable<bool> &detached = mine.detached[lhs.indx[SubLevel]];
                        bool                  keep     = true;
                        for(size_t j=1;j<i;++j)
                        {
                            const Equilibrium &rhs = trades[j].eq;
                            if(!detached[rhs.indx[SubLevel]])
                            {
                                keep = false;
                                break;
                            }
                        }
                        if(keep)
                        {
                            Y_XMLOG(xml,"(+) " << tri);
                            ++i;
                        }
                        else
                        {
                            Y_XMLOG(xml,"(-) " << tri);
                            trades.remove(i);
                        }
                    }
                }


                //--------------------------------------------------------------
                //
                // apply found trades
                //
                //--------------------------------------------------------------
                {
                    Y_XML_SECTION_OPT(xml, "applied", "size='" << trades.size() << "'");
                    const size_t nt = trades.size(); assert(nt>=1);
                    if(xml.verbose)
                    {
                        for(size_t i=1;i<=nt;++i)
                        {
                            xml() << "($) " << trades[i] << std::endl;
                        }
                    }

                    if(nt<=1)
                    {
                        //------------------------------------------------------
                        // default
                        //------------------------------------------------------
                        mine.transfer(C, L, top.cc, SubLevel);
                    }
                    else
                    {
                        const AddressBook &cdb = mine.conserved.book;

                        //------------------------------------------------------
                        // initialize accumulator
                        //------------------------------------------------------
                        xaccum.forEach( &XAdd::free );
                        for(const SNode *sn=mine.unbounded.list.head;sn;sn=sn->next)
                        {
                            const Species &      sp = **sn;
                            const size_t * const id = sp.indx;
                            xaccum[ id[SubLevel] ] << C[ id[L] ];
                        }

                        //------------------------------------------------------
                        // dispatch species
                        //------------------------------------------------------
                        for(size_t i=1;i<=nt;++i)
                        {
                            const Trade       &tr = trades[i];
                            const Equilibrium &eq = tr.eq;
                            const XReadable   &cc = tr.cc;
                            const XReadable   &dc = tr.dc;
                            size_t n = eq->size();
                            for(Equilibrium::ConstIterator it=eq->begin();n>0;--n,++it)
                            {
                                const Species &      sp = (**it).sp;
                                const size_t * const id = sp.indx;
                                const size_t         jj = id[SubLevel];
                                if(cdb.has(sp))
                                {
                                    // conserved species => direct transfer
                                    C[ id[L] ] = cc[jj];
                                }
                                else
                                {
                                    // unbounded species => collect
                                    xaccum[jj] << dc[jj];
                                }
                            }
                        }

                        //------------------------------------------------------
                        // collect unbounded
                        //------------------------------------------------------
                        for(const SNode *sn=mine.unbounded.list.head;sn;sn=sn->next)
                        {
                            const Species &      sp = **sn;
                            const size_t * const id = sp.indx;
                            C[ id[L] ] = xaccum[ id[SubLevel] ].sum();
                        }

                        if(false)
                        {

                            for(const SNode *sn = mine.species.head; sn; sn=sn->next)
                            {
                                const Species &sp = **sn;
                                std::cerr << std::setw(15) << real_t( C[ sp.indx[L]]) << " = [" << sp << "]" <<  std::endl;
                            }
                            exit(7);
                        }
                    }
                }




            }

            size_t getUnbalanced(const XReadable &C, const Level L, XMLog &xml)
            {
                Y_XML_SECTION(xml, "getUnbalanced");

                //--------------------------------------------------------------
                //
                // initialize
                //
                //--------------------------------------------------------------
                size_t             unbalanced = 0;
                const AddressBook &cdb  = mine.conserved.book;
                trades.free();
                wobbly.free();

                //--------------------------------------------------------------
                //
                // loop over limited equilibria
                //
                //--------------------------------------------------------------
                for(const ENode *en=mine.limited.head;en;en=en->next)
                {
                    //----------------------------------------------------------
                    // study equilibrium
                    //----------------------------------------------------------
                    const Equilibrium &eq    = **en;
                    const Trims::Kind  kind  = trms(C,L,eq,cdb);
                    best.free();

                    switch(kind)
                    {
                        case Trims::BadNone: Y_XMLOG(xml, "(+) " << eq);
                            continue; // next

                        case Trims::BadBoth: Y_XMLOG(xml, "(-) " << eq);
                            ++unbalanced;
                            Y_XMLOG(xml, " |_reac: " << trms.reac.required);
                            Y_XMLOG(xml, " |_prod: " << trms.prod.required);
                            continue; // next

                        case Trims::BadProd:Y_XMLOG(xml, "(>) " << eq);
                            ++unbalanced;
                            Y_XMLOG(xml, " |_prod: " << trms.prod.required);
                            Y_XMLOG(xml, " |_reac: " << trms.reac.limiting);
                            if(trms.reac.limiting.blocking())
                            {
                                Y_XMLOG(xml, " |_blocked...");
                                continue; // next
                            }
                            else
                            {
                                bestEffort(trms.reac.limiting, trms.prod.required);
                                break; // will move
                            }


                        case Trims::BadReac:Y_XMLOG(xml, "(<) " << eq);
                            ++unbalanced;
                            Y_XMLOG(xml, " |_reac: " << trms.reac.required);
                            Y_XMLOG(xml, " |_prod: " << trms.prod.limiting);
                            if(trms.prod.limiting.blocking())
                            {
                                Y_XMLOG(xml, " |_blocked...");
                                continue; // next
                            }
                            else
                            {
                                bestEffort(trms.prod.limiting, trms.reac.required);
                                Coerce(best.xi).neg();
                                break; // will move
                            }
                    }

                    assert( best->size > 0 );
                    Y_XMLOG(xml, " |_best@" << best);

                    //----------------------------------------------------------
                    // setup a new trade
                    //----------------------------------------------------------
                    const xreal_t _0;
                    const size_t  ii = trades.size()+1;
                    XWritable   & cc = mine.transfer(ctrade[ii],SubLevel,C,L);
                    XWritable   & dc = dtrade[ii].ld(_0);

                    //----------------------------------------------------------
                    // update cc
                    //----------------------------------------------------------
                    xadd.free();
                    {
                        const xreal_t xi = best.xi;
                        size_t        n  = eq->size();
                        for(Components::ConstIterator it=eq->begin();n>0;++it,--n)
                        {
                            const Component &cm = **it;
                            const Species   &sp = cm.sp;
                            const size_t     jj = sp.indx[SubLevel];
                            const xreal_t    c0 = cc[jj];
                            const xreal_t    c1 = c0 + xi * cm.xn;
                            const bool       conserved = cdb.has(sp);

                            // careful update
                            if(c0.mantissa<0)
                            {
                                xadd << -c0;
                                cc[jj] = c1;
                            }
                            else
                            {
                                if(conserved)
                                {
                                    // tailored to keep correct
                                    cc[jj] = c1.mantissa>=0 ? c1 : _0;
                                }
                                else
                                {
                                    // whatever
                                    cc[jj] = c1;
                                }

                            }
                        }
                    }

                    //----------------------------------------------------------
                    // force vanishing
                    //----------------------------------------------------------
                    for(const SNode *sn=best->head;sn;sn=sn->next)
                    {
                        cc[ (**sn).indx[SubLevel] ] = _0;
                    }

                    //----------------------------------------------------------
                    // update negative score and dc
                    //----------------------------------------------------------
                    {
                        size_t        n  = eq->size();
                        for(Components::ConstIterator it=eq->begin();n>0;++it,--n)
                        {
                            const Species &sp = (**it).sp;
                            const size_t   jj = sp.indx[SubLevel];
                            const xreal_t  c1 = cc[ jj ];
                            if(c1.mantissa<0) xadd << c1;
                            dc[jj] = c1 - C[ sp.indx[L] ];
                        }
                    }

                    //----------------------------------------------------------
                    // append trade
                    //----------------------------------------------------------
                    {
                        const Trade trade(eq,cc,xadd.sum(),dc);
                        trades << trade;
                    }

                    Y_XMLOG(xml," |_gain: " << trades.tail());
                }

                //--------------------------------------------------------------
                //
                // finalize
                //
                //--------------------------------------------------------------
                assert(0==wobbly.size);
                if(unbalanced>0)
                {
                    for(const SNode *sn=mine.conserved.list.head;sn;sn=sn->next)
                    {
                        const Species &sp = **sn;
                        assert( cdb.has(sp) );
                        if( C[ sp.indx[L] ].mantissa >= 0 ) continue;
                        wobbly << sp;
                    }
                    return unbalanced;
                }
                else
                {
                    return 0;
                }
            }

            void bestEffort(const SingleFrontier &limiting,
                            const Frontiers      &required)
            {
                assert(required.size>0);        // number of frontiers
                assert(limiting->size>0);       // number of limiting species
                assert(limiting.xi.mantissa>0); // positive limiting extent

                best.free();
                const  Frontier *prev = 0;
                for(const FNode *node=required.head;node;node=node->next)
                {
                    const Frontier &F = **node;
                    switch( Sign::Of(F.xi,limiting.xi) )
                    {
                        case Negative:
                            prev = &F; // almost to this oe
                            continue;

                        case __Zero__:
                            Coerce(best.xi) = limiting.xi;
                            best << limiting;
                            best << F;
                            return;

                        case Positive:
                            break; // will stop
                    }
                    break; // stop
                }

                if(0!=prev)
                {
                    //----------------------------------------------------------
                    //
                    // will solve up to prev
                    //
                    //----------------------------------------------------------
                    Coerce(best.xi) = prev->xi;
                    best << *prev;
                }
                else
                {
                    //----------------------------------------------------------
                    //
                    // best partial effort
                    //
                    //----------------------------------------------------------
                    Coerce(best.xi) = limiting.xi;
                    best   << limiting;
                }


            }

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
                const Conservation::Law *law = 0; assert( jail.size() > 0);
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
            std::cerr << std::endl << "--" << std::endl;
            plexus.conc(C0,0.3,0.5);
            warden.prolog();
            lib(std::cerr << "C0=","\t[",C0,"]");
            if( warden.sanitize(C0,TopLevel,xml) )
            {
                warden.equalize(C0,TopLevel,xml);
            }
            lib(std::cerr << "C1=","\t[",C0,"]");

        }


    }
#endif


    Y_SIZEOF(Warden);
}
Y_UDONE()

