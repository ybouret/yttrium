
#include "y/chem/plexus/janitor.hpp"
#include "y/data/small/heavy/list/coop.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Janitor:: ~Janitor() noexcept
        {
        }

        Janitor:: Janitor()
        {

        }


        class Cursor : public SpStrip
        {
        public:
            xreal   xi; //!< xi value

            inline explicit Cursor(const SpProxy &_) : SpStrip(_), xi(0) {}
            inline          Cursor(const Cursor  &cursor) : SpStrip(cursor), xi(cursor.xi) {}
            inline virtual ~Cursor() noexcept {}

        private:
            Y_DISABLE_ASSIGN(Cursor);
        };

        typedef Small::CoopHeavyList<Cursor> Cursors;
        typedef Cursors::ProxyType           CursorsProxy;

        class Inquiry
        {
        public:
            inline explicit Inquiry(const CursorsProxy &_) noexcept : limiting(_), equating(_) { }
            inline virtual ~Inquiry()                      noexcept {}


            Cursors limiting;
            Cursors equating;


            inline void probe(const Actors          &actors,
                              const Readable<xreal> &Corg,
                              const Booleans        &kept)
            {
                limiting.free();
                equating.free();
                for(const Actor *a=actors.head;a;a=a->next)
                {
                    const Species &sp = a->sp;
                    if(!kept[sp.indx[SubLevel]]) continue;
                    const double c = Corg[sp.indx[TopLevel]];
                    if(c>=0)
                    {
                        // limit
                    }
                    else
                    {
                        // slide
                    }
                }
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Inquiry);
        };



        void Janitor:: process(const Cluster   &cluster,
                               Writable<xreal> &Corg,
                               XMLog           &xml)
        {
            Y_XML_SECTION(xml, "Janitor::Cluster");

            CursorsProxy cproxy;
            Inquiry      reac(cproxy);
            Inquiry      prod(cproxy);

            const Booleans &kept = *cluster.kept;
            const EqRepo &repo = cluster.army->definite;
            for(const EqNode *en=repo.head;en;en=en->next)
            {
                const Equilibrium &eq = **en;
                reac.probe(eq.reac, Corg, kept);
                prod.probe(eq.prod, Corg, kept);
            }


        }
    }

}
