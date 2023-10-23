
#include "y/chem/plexus/janitor.hpp"

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

            Cursor(const SpProxy &_) : SpStrip(_), xi(0) {}
            ~Cursor() noexcept {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Cursor);
        };


        static inline void Probe(const Actors          &actors,
                                 const Readable<xreal> &Corg,
                                 const Booleans        &kept)
        {
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

        void Janitor:: process(const Cluster   &cluster,
                               Writable<xreal> &Corg,
                               XMLog           &xml)
        {
            Y_XML_SECTION(xml, "Janitor::Cluster");
            const Booleans &kept = *cluster.kept;
            const EqRepo &repo = cluster.army->definite;
            for(const EqNode *en=repo.head;en;en=en->next)
            {
                const Equilibrium &eq = **en;
                Probe(eq.reac,Corg,kept);
            }


        }
    }

}
