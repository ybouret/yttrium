
#include "y/chemical/reactive/plexus/warden.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        namespace Conservation
        {
            Warden:: Broken:: Broken(const Broken &other) noexcept :
            law(other.law),
            bad(other.bad)
            {
            }

            Warden:: Broken:: ~Broken() noexcept
            {
            }

            Warden:: Broken:: Broken(const Law &L, const xreal_t X) noexcept :
            law(L), bad(X)
            {
            }
            
            std::ostream & operator<<(std::ostream &os, const Warden::Broken &self)
            {
                os << self.law << " = " << sqrt(real_t(self.bad));
                return os;
            }

            Warden:: Warden(const Clusters &clusters) :
            dC(clusters.maxCPG,clusters.maxSPG),
            repo(),
            jail(repo),
            xadd(clusters.maxSPG)
            {
                repo->reserve(clusters.maxCPG);
                std::cerr << "dC=" << dC << std::endl;
            }

            Warden:: ~Warden() noexcept
            {
            }

            void Warden:: process(Writable<xreal_t>  &C,
                                  Writable<xreal_t>  &I,
                                  const Group        &G)
            {
                static const xreal_t zero(0);
                const size_t         nlaw = G->size; assert(nlaw>0); assert(nlaw<=dC.rows);
                jail.free();                         assert(repo->stowage()>=nlaw);

                // init
                for(const LawNode *node=G->head;node;node=node->next)
                {
                    const Law    &law = **node;
                    const xreal_t xs  = law.required(dC[jail.size+1],AuxLevel, C, TopLevel, xadd);
                    //std::cerr << law << " = " << sqrt(real_t(xs)) << " => " << dC[jail.size+1] << std::endl;
                    if(xs>zero)
                    {
                        const Broken broken(law,xs);
                        jail << broken;
                        
                    }
                }
                std::cerr << "jail=" << jail << std::endl;



            }

            void Warden:: operator()(Writable<xreal_t>  &C,
                                     Writable<xreal_t>  &I,
                                     const GList        &G)
            {
                I.ld(0);
                for(const GNode *grp =G.head;grp;grp=grp->next)
                {
                    process(C,I,**grp);
                }
            }

        }


    }

}


