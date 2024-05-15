#include "y/chemical/plexus/equalizer.hpp"

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
        negative(banks.ls),
        fences(clusters.maxCPC,banks)
        {
        }

        void Equalizer:: tune(XWritable     &C0,
                              const Cluster &cluster,
                              XMLog         &xml)
        {

            Y_XML_SECTION(xml, "Chemical::Equalizer::Tune");
            const xreal_t zero;

            //__________________________________________________________________
            //
            //
            // initialize negative concentrations
            //
            //__________________________________________________________________
            negative.free();
            for(const SNode *node=cluster.species.head;node;node=node->next)
            {
                const Species &sp = **node;
                if( C0[ sp.indx[TopLevel]] < zero ) negative << sp;
            }
            Y_XMLOG(xml,"negative=" << negative);
            if(negative.size<=0) return;

            //__________________________________________________________________
            //
            //
            // first part:
            //
            //__________________________________________________________________
            const Controllers &controllers = cluster.controllers;
            if(controllers.size>0)
            {
                Y_XMLOG(xml, "using controllers" );
                size_t active = 0;
                for(const Controller *cntl=controllers.head;cntl;cntl=cntl->next)
                {
                    const size_t  pos  = active+1;
                    Fence       &fence = fences[pos];
                    fence.shapeFull(cntl->components,C0);
                    Y_XMLOG(xml,fence);
                }
            }



        }


    }

}

