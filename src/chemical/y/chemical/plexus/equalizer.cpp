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
            if(negative.size<=0)
            {
                Y_XMLOG(xml,"no negative concentration");
                return;
            }
            else
            {
                Y_XMLOG(xml,"negative=" << negative);
            }

            //__________________________________________________________________
            //
            //
            // first part: scanning controllers
            //
            //__________________________________________________________________
            const Controllers &controllers = cluster.controllers;
            const SList       &species     = cluster.species;
            if(controllers.size>0)
            {
                Y_XML_SECTION(xml, "Controllers");
                size_t active = 0;
                for(const Controller *cntl=controllers.head;cntl;cntl=cntl->next)
                {
                    //__________________________________________________________
                    //
                    // scan current controller
                    //__________________________________________________________
                    const Equilibrium &eq = cntl->primary;
                    Y_XML_SECTION(xml, eq.name);
                    const size_t   index  = active+1;
                    Fence         &fence = fences[index].shapeFull(cntl->components,C0);
                    Y_XMLOG(xml, " (*) " << fence);
                    const unsigned state = fence.studyFull(xml);
                    const unsigned flags = (state&Fence::ST_MASK);
                    switch( flags )
                    {
                        case Fence::RUNNING: goto CONTINUE;
                        case Fence::BLOCKED: goto CONTINUE;
                        case Fence::EQUATED:
                            break;
                        case Fence::PARTIAL:
                            break;
                        default:
                            throw Exception("Flags %u Not Handled", flags);
                    }

                    //__________________________________________________________
                    //
                    // process controller with outcome
                    //__________________________________________________________
                    {
                        XWritable         &C1 = Ceqz[index];
                        cluster.equalize(C1, SubLevel, eq, fence.cursor, fence.zeroed.head, C0, TopLevel);
                        if(xml.verbose)
                        {
                            for(const SNode *node=species.head;node;node=node->next)
                            {
                                const Species &sp = **node;
                                if(!eq.contains(sp)) continue;
                                cluster.spfmt.pad( xml() << " (|) " << sp,sp) << " = ";
                                *xml << std::setw(15) << real_t( C0[ sp.indx[TopLevel]] ) << " -> ";
                                *xml << std::setw(15) << real_t( C1[ sp.indx[SubLevel]] );
                                *xml << std::endl;
                            }
                        }
                    }

                CONTINUE:;
                }
            }



        }


    }

}

