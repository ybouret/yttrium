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
            const xreal_t zero;
            negative.free();
            for(const SNode *node=cluster.conservedSpecies.head;node;node=node->next)
            {
                const Species &sp = **node; assert( cluster.isLimited(sp) );
                if( C0[ sp.indx[TopLevel] ] < zero ) negative += sp;
            }

            Y_XML_SECTION_OPT(xml, "Controllers"," negative='" << negative.size() << "'");
            if(negative.size()<=0) return;

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

