
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


        class Cursor
        {
        public:
            xreal   xi;
            SpStrip sp;

            Cursor(const SpProxy &_) : xi(0), sp(_) {}
            ~Cursor() noexcept {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Cursor);
        };


        void Janitor:: process(const Cluster   &cluster,
                               Writable<xreal> &Corg)
        {

            const EqRepo &repo = cluster.army->definite;
            for(const EqNode *en=repo.head;en;en=en->next)
            {
                const Equilibrium &eq = **en;
                
            }


        }
    }

}
