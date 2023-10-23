
#include "y/chem/plexus/janitor.hpp"
#include "y/data/small/heavy/list/coop.hpp"
#include "y/type/nullify.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        namespace
        {

            //__________________________________________________________________
            //
            //
            //! Cursor for vanishing value/species
            //
            //__________________________________________________________________
            class Cursor : public SpStrip
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                inline explicit Cursor(const SpProxy &_) : SpStrip(_), xi(0) {}
                inline explicit Cursor(const SpProxy &_, const Species &s, const xreal &x) : SpStrip(_), xi(x) { (*this) << s; }
                inline          Cursor(const Cursor  &cursor) : SpStrip(cursor), xi(cursor.xi) {}
                inline virtual ~Cursor() noexcept {}

                //______________________________________________________________
                //
                // Methods
                //______________________________________________________________

                //! full reset
                inline void reset() noexcept { xi = 0; free(); }

                //! set one species at given extent
                inline void start(const xreal &x, const Species &s)
                {
                    free(); xi = x; (*this) << s;
                }

                //! display
                friend inline std::ostream & operator<<(std::ostream &os, const Cursor &self)
                {
                    if(self.size>0)
                    {
                        const SpStrip &me = self;
                        os << me << "@" << double(self.xi);
                    }
                    else
                    {
                        os << "none";
                    }
                    return os;
                }

                //______________________________________________________________
                //
                // Members
                //______________________________________________________________
                xreal   xi; //!< xi value


            private:
                Y_DISABLE_ASSIGN(Cursor);
            };


            typedef Small::CoopHeavyList<Cursor> Cursors;
            typedef Cursors::ProxyType           CursorsProxy;

            class Inquiry
            {
            public:
                inline   Inquiry(const SpProxy      &sprx,
                                 const CursorsProxy &cprx) noexcept :
                limiting(sprx),
                negative(cprx)
                {
                }

                inline ~Inquiry() noexcept
                {
                }

                Cursor  limiting;
                Cursors negative;

                inline void probe(const Actors          &actors,
                                  const Readable<xreal> &Corg,
                                  const Booleans        &kept)
                {
                    limiting.reset();
                    for(const Actor *a=actors.head;a;a=a->next)
                    {
                        const Species &sp = a->sp;
                        if(!kept[sp.indx[SubLevel]]) continue;

                        const xreal c = Corg[sp.indx[TopLevel]];
                        if(c.mantissa>=0)
                        {
                            //--------------------------------------------------
                            // limiting concentration
                            //--------------------------------------------------
                            const xreal xi = c/a->xn;

                            switch(limiting.size)
                            {
                                case 0:
                                    limiting.xi = xi;
                                    limiting << sp;
                                    break;

                                default:
                                    switch( Sign::Of(xi,limiting.xi))
                                    {
                                        case Negative: limiting.start(xi,sp); break; // new winner
                                        case Positive: break;                        // not better
                                        case __Zero__: limiting << sp; break;        // multiple winner
                                    }

                            }

                        }
                        else
                        {
                            //--------------------------------------------------
                            // equating concentration
                            //--------------------------------------------------
                            const xreal xi = (-c)/a->xn;
                            Cursor      cr(limiting.proxy,sp,xi);
                            upgradeWith(cr);
                        }
                    }
                }

                friend inline std::ostream & operator<<(std::ostream &os, const Inquiry &self)
                {
                    os << " | limiting: " << self.limiting;
                    if(self.negative.size>0)
                        os << " | negative: " << self.negative;
                    return os;
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Inquiry);

                inline void upgradeWith(Cursor &cr)
                {
                    for(Cursors::NodeType *node=negative.head;node;node=node->next)
                    {
                        Cursor &it = **node;
                        switch( Sign::Of(cr.xi,it.xi) )
                        {
                            case Negative: 
                                // smaller than current value => before
                                negative.insertBefore(node, negative.generate(cr) );
                                return;

                            case __Zero__: 
                                // same than current value => merge
                                it.mergeTail(cr);
                                return;

                            case Positive: 
                                // greater thant current value => after
                                continue;
                        }
                    }
                    // bigger than all, append a copy
                    negative << cr;
                }
            };



        }

        //______________________________________________________________________
        //
        //
        //
        //! Janitor Code
        //
        //
        //______________________________________________________________________
        class Janitor:: Code : public Object
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            inline explicit Code() :
            Object(),
            sprx(),
            cprx(),
            reac(sprx,cprx),
            prod(sprx,cprx)
            {
            }

            inline virtual ~Code() noexcept {}

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            inline void probe(const Equilibrium     &eq,
                              const Readable<xreal> &Corg,
                              const Booleans        &kept,
                              XMLog                 &xml)
            {
                Y_XMLOG(xml, "probing " << eq);
                reac.probe(eq.reac, Corg, kept);
                prod.probe(eq.prod, Corg, kept);
                Y_XMLOG(xml," reac" << reac);
                Y_XMLOG(xml," prod" << prod);
            }


            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            SpProxy      sprx;
            CursorsProxy cprx;
            Inquiry      reac;
            Inquiry      prod;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);
        };


        Janitor:: ~Janitor() noexcept
        {
            assert(0!=code);
            Nullify(code);
        }

        Janitor:: Janitor() : code( new Code() )
        {

        }

        void Janitor:: process(const Cluster   &cluster,
                               Writable<xreal> &Corg,
                               XMLog           &xml)
        {
            Y_XML_SECTION(xml, "Janitor::Cluster");
            assert(0!=code);


            const Booleans &kept = *cluster.kept;
            const EqRepo   &repo =  cluster.army->definite;
            for(const EqNode *en=repo.head;en;en=en->next)
            {
                const Equilibrium &eq = **en;
                code->probe(eq,Corg,kept,xml);
            }


        }
    }

}
