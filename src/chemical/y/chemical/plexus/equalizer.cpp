#include "y/chemical/plexus/equalizer.hpp"
#include "y/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Equalizer:: ~Equalizer() noexcept
        {
        }

        Equalizer:: Equalizer(const Clusters &clusters) :
        Ceqz( clusters.maxCPC, clusters.maxSPC),
        banks(),
        negative(),
        fences(clusters.maxCPC,banks),
        xadd(),
        bank()
        {
        }


        void Equalizer:: tuneControllers(XWritable     &C0,
                                         const Cluster &cluster,
                                         XMLog         &xml)

        {
            //__________________________________________________________________
            //
            //
            // initialize
            //
            //__________________________________________________________________
            const xreal_t zero;
            const SList  &species = cluster.conservedSpecies;
            if(species.size<=0) return;

            //__________________________________________________________________
            //
            //
            // probing negative conserved species
            //
            //__________________________________________________________________
            negative.free();
            for(const SNode *node=species.head;node;node=node->next)
            {
                const Species &sp = **node; assert( cluster.isLimited(sp) );
                if( C0[ sp.indx[TopLevel] ] < zero ) negative += sp;
            }

            Y_XML_SECTION_OPT(xml, "Controllers"," negative='" << negative.size() << "' conserved='" << species.size);
            if(negative.size()<=0)
                return;

            if(xml.verbose) negative.display<Species>( xml() <<"negative=") << std::endl;
            //__________________________________________________________________
            //
            //
            // probing negative conserved species
            //
            //__________________________________________________________________
            size_t active = 0;
            for(const Controller *cntl=cluster.controllers.head;cntl;cntl=cntl->next)
            {
                const Equilibrium &eq = cntl->primary;
                const Components  &cm = cntl->components;
                if( ! cm.isConcernedBy(negative) )
                {
                    Y_XMLOG(xml," (-) " << eq);
                    continue;
                }

                Y_XMLOG(xml, " (+) " << eq);

                const size_t   index = active+1;
                Fence &        fence = fences[index];
                const unsigned state = fence(cm,C0,xml);
                
            }

        }

        void Equalizer:: tune(XWritable     &C0,
                              const Cluster &cluster,
                              XMLog         &xml)
        {

            Y_XML_SECTION(xml, "Chemical::Equalizer::Tune");

            tuneControllers(C0,cluster,xml);

        }


    }

}

