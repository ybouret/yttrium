
#include "y/chem/plexus/equalizer.hpp"
#include "y/data/small/heavy/list/coop.hpp"
#include "y/type/nullify.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        namespace
        {

            class Cursor : public SpStrip
            {
            public:
                inline explicit Cursor(const SpProxy &_) noexcept : SpStrip(_),     xi(0)        {} //!< init 0
                inline          Cursor(const Cursor &other)       : SpStrip(other), xi(other.xi) {} //!< copy
                inline explicit Cursor(const SpProxy &_,
                                       const Species &s,
                                       const xreal   &x) :
                SpStrip(_),
                xi(x)
                {
                    (*this) << s;
                }

                inline virtual ~Cursor() noexcept {}                                                //!< cleanup

                inline void init() noexcept { xi=0; free(); }

                inline void make(const Species &s, const xreal &x)
                {
                    assert(x.mantissa>=0);
                    SpStrip &me = *this;
                    if(size<=0)
                    {
                        me << s;
                        xi =  x;
                    }
                    else
                    {
                        switch( Sign::Of(x,xi) )
                        {
                            case Negative:
                                xi = x;
                                me.free();
                                me << s;
                                break;

                            case __Zero__:
                                me << s;
                                break;

                            case Positive:
                                break;
                        }
                    }
                    assert(size>0);
                }

                inline friend std::ostream & operator<<(std::ostream &os, const Cursor &self)
                {
                    if(self.size<=0)
                        os << "none";
                    else
                    {
                        const SpStrip &me = self;
                        os << double(self.xi) << "@" << me;
                    }
                    return os;
                }

                xreal xi;
            private:
                Y_DISABLE_ASSIGN(Cursor);
            };

            typedef Small::CoopHeavyList<Cursor> CursorsList;
            typedef CursorsList::ProxyType       CursorsProxy;
            typedef CursorsList::NodeType        CursorNode;

            class Cursors : public CursorsList
            {
            public:
                inline explicit Cursors(const CursorsProxy &_) noexcept : CursorsList(_) {}
                inline virtual ~Cursors() noexcept {}

                inline void use(Cursor &cr)
                {
                    CursorsList &self = *this;

                    // get rid of trivial cases
                    switch(size)
                    {
                        case 0: self << cr; return;
                        case 1:
                            switch( Sign::Of(cr.xi, (**head).xi) )
                            {
                                case Negative: self >> cr; return;
                                case __Zero__: (**head).mergeTail(cr); return;
                                case Positive: self << cr; return;
                            }
                            return;

                        default:
                            break;
                    }

                    // look up in pre-ordered list
                    for(CursorNode *node=head;node;node=node->next)
                    {
                        Cursor  &here= **node;
                        switch( Sign::Of(cr.xi,here.xi) )
                        {
                            case Negative: insertBefore(node,generate(cr)); return;
                            case __Zero__: here.mergeTail(cr);              return;
                            case Positive: continue;
                        }
                    }
                    self << cr;

                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Cursors);
            };

            class Boundary
            {
            public:
                inline explicit Boundary(const SpProxy      &sprx,
                                         const CursorsProxy &cprx) noexcept :
                lim(sprx),
                eqz(cprx)
                {
                }

                inline virtual ~Boundary() noexcept
                {
                }

                inline void init() noexcept
                {
                    lim.init(); assert(0==lim.size);
                    eqz.free(); assert(0==eqz.size);
                }

                inline void probe(const Actors          &actors,
                                  const Booleans        &kept,
                                  const Readable<xreal> &Corg)
                {
                    init();
                    for(const Actor *a=actors.head;a;a=a->next)
                    {
                        const Species &sp = a->sp;
                        if( !kept[sp.indx[SubLevel]]) continue;
                        const xreal c = Corg[sp.indx[TopLevel]];
                        if(c.mantissa>=0)
                        {
                            // update lim
                            lim.make(sp,c/a->xn);
                        }
                        else
                        {
                            // update eqz
                            Cursor cr(lim.proxy,sp,(-c)/a->xn);
                            eqz.use(cr);
                        }
                    }
                }

                inline friend std::ostream & operator<<(std::ostream &os, const Boundary &self)
                {
                    os << " | lim:" << self.lim << " | eqz:";
                    if(self.eqz.size>0) os << self.eqz; else os << "none";
                    return os;
                }

                Cursor  lim;
                Cursors eqz;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Boundary);
            };
            

        }

        class Equalizer:: Code : public Object
        {
        public:
            inline explicit Code() : Object(),
            sprx(),
            cprx(),
            reac(sprx,cprx),
            prod(sprx,cprx),
            Ctry()
            {}

            inline virtual ~Code() noexcept {}

            inline void prepare(const Plexus &plexus, 
                                XMLog        &xml)
            {
                Y_XML_SECTION(xml,"Equalizer::Prepare");
                Ctry.make(plexus.maxEqzSz,plexus.maxSubId);
            }

            inline void run(const Cluster   &cluster,
                            Writable<xreal> &Corg,
                            XMLog           &xml)
            {
                const EqRepo   &repo = cluster.army->definite;
                const Booleans &kept = *cluster.kept;
                Y_XML_SECTION_OPT(xml, "Equalize::Cluster", " definite='" << repo.size << "'");
                for(const EqNode *en=repo.head;en;en=en->next)
                {
                    const Equilibrium &eq = **en;
                    reac.probe(eq.reac,kept,Corg);
                    prod.probe(eq.prod,kept,Corg);

                    Y_XMLOG(xml,eq << " @aux=" << eq.indx[AuxLevel]);
                    Y_XMLOG(xml, "|_reac: " << reac);
                    Y_XMLOG(xml, "|_prod: " << prod);

                    eq.display(std::cerr,Corg,TopLevel,Conv::X2R) << std::endl;


                }

            }

            SpProxy       sprx;
            CursorsProxy  cprx;
            Boundary      reac;
            Boundary      prod;
            Matrix<xreal> Ctry;


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);
        };


    }

}



namespace Yttrium
{
    namespace Chemical
    {

        Equalizer:: ~Equalizer() noexcept
        {
            assert(0!=code);
            Nullify(code);
        }

        Equalizer:: Equalizer() : code( new Code() )
        {

        }

        void Equalizer:: run(const Cluster   &cluster,
                             Writable<xreal> &Corg,
                             XMLog           &xml)
        {
            assert(0!=code);
            code->run(cluster,Corg,xml);
        }


        void Equalizer:: prepare(const Plexus &plexus, XMLog &xml)
        {
            assert(0!=code);
            code->prepare(plexus,xml);
        }
    }

}
