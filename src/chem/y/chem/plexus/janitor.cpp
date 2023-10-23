
#include "y/chem/plexus/janitor.hpp"
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
                xreal   xi; //!< xi value
                inline explicit Cursor(const SpProxy &_) : SpStrip(_), xi(0) {}
                inline          Cursor(const Cursor  &cursor) : SpStrip(cursor), xi(cursor.xi) {}
                inline virtual ~Cursor() noexcept {}

                inline void reset() noexcept { xi = 0; free(); }

            private:
                Y_DISABLE_ASSIGN(Cursor);
            };


            class Inquiry
            {
            public:
                inline   Inquiry(const SpProxy &sprx) noexcept :
                limiting(sprx)
                {
                }

                inline ~Inquiry() noexcept
                {
                }

                Cursor limiting;

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
                            // limiting
                            const xreal xi = c/a->xn;
                            std::cerr << "limiting by xi_" << sp.name << " = " << double(xi) << std::endl;

                            switch(limiting.size)
                            {
                                case 0:
                                    limiting.xi = xi;
                                    limiting << sp;
                                    break;

                                default:
                                    ;
                            }




                        }
                        else
                        {
                            // equating
                            const xreal xi = (-c)/a->xn;
                            std::cerr << "equating by xi_" << sp.name << " = " << double(xi) << std::endl;
                        }
                    }
                }

                friend inline std::ostream & operator<<(std::ostream &os, const Inquiry &self)
                {
                    if(self.limiting.size>0)
                    {
                        os << "limiting: " << self.limiting;
                    }


                    return os;
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Inquiry);
            };


#if 0
            typedef Small::CoopHeavyList<Cursor> Cursors;
            typedef Cursors::ProxyType           CursorsProxy;
            typedef Cursors::NodeType            CursorNode;

            class Inquiry
            {
            public:
                inline explicit Inquiry(const CursorsProxy &_) noexcept : limiting(_), equating(_) { }
                inline virtual ~Inquiry()                      noexcept {}


                Cursor  limiting;
                Cursors equating;





            private:
                Y_DISABLE_COPY_AND_ASSIGN(Inquiry);
            };
#endif

        }

        //______________________________________________________________________
        //
        //
        //
        //! Janitro Code
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
            reac(sprx),
            prod(sprx)
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
                Y_XMLOG(xml," reac: " << reac);
                Y_XMLOG(xml," prod: " << prod);
            }


            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            SpProxy sprx;
            Inquiry reac;
            Inquiry prod;

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
