#include "y/chemical/plexus/equalizer.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Equalizer:: ~Equalizer() noexcept
        {
        }

        Equalizer:: Equalizer(const Clusters &clusters) :
        Ceqz(clusters.maxCPC,clusters.maxSPC),
        banks(),
        fences(clusters.maxCPC,banks),
        negative(),
        xadd(),
        bank()
        {
        }


        void Equalizer:: tuneControllers(XWritable     &C0,
                                         const Cluster &cluster,
                                         XMLog         &xml)

        {
            static const char here[] = "Chemical::Equalizer::tuneControllers";

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

            Y_XML_SECTION_OPT(xml,here," negative='" << negative.size() << "' conserved='" << species.size);
            if(negative.size()<=0)
                return;

            if(xml.verbose) negative.display<Species>( xml() <<"negative=") << std::endl;

            FList flist(bank);

            //__________________________________________________________________
            //
            //
            // looping over concerned controllers
            //
            //__________________________________________________________________
        CYCLE:
            flist.free();
            for(const Controller *cntl=cluster.controllers.head;cntl;cntl=cntl->next)
            {
                const Equilibrium &eq = cntl->primary;
                const Components  &cm = cntl->components;
                if( ! cm.isConcernedBy(negative) )
                {
                    Y_XMLOG(xml," (-) " << eq);
                    continue;
                }

                //______________________________________________________________
                //
                // current controller may handle a negative concentration
                //______________________________________________________________
                if(xml.verbose) cluster.eqfmt.print( xml() << " (+) " << eq << " : ", eq) << std::endl;


                const size_t   index = flist.size+1;           // new index
                Fence         &fence = fences[index];          // get a new fence
                const unsigned state = fence(cm,C0,xml);       // build the fence
                const unsigned flags = (state&Fence::ST_MASK); // get the flags

                //______________________________________________________________
                //
                // act upon result
                //______________________________________________________________
                switch( flags )
                {
                    case Fence::BLOCKED: goto CONTINUE;

                    case Fence::PARTIAL:
                        break;

                    case Fence::EQUATED:
                        break;

                    default:
                        throw Specific::Exception(here,"corrupted flags='%s'", Fence::StatusText(flags) );
                }

                //______________________________________________________________
                //
                // compute and store improved state
                //______________________________________________________________
                assert(0!=(flags&Fence::IMPROVE));
                {
                    XWritable    &C1 = Ceqz[index];
                    const xreal_t g1 = cluster.equalized(C1, SubLevel, eq, fence.cursor, fence.zeroed.head, C0, TopLevel, xadd);
                    const Fixed   f(g1,C1,*cntl,fence);
                    flist << f;
                    if(xml.verbose)
                    {
                        for(Components::ConstIterator it=eq->begin();it!=eq->end();++it)
                        {
                            const Species &sp = (*it).sp;
                            cluster.spfmt.pad( xml() << " (|) " << sp, sp)
                            << " : "   << std::setw(15) << real_t(C0[sp.indx[TopLevel]])
                            << " -> "  << std::setw(15) << real_t(C1[sp.indx[SubLevel]])
                            << std::endl;
                        }
                        xml() << " ($) " << real_t(g1) << std::endl;
                    }
                }

            CONTINUE:;
            }

            Y_XMLOG(xml, " (#) fixed=" << flist.size);
            
            if(flist.size<=0)
            {

            }

            return;
            goto CYCLE;

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

