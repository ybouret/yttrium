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
        fence(banks),
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
           
            //__________________________________________________________________
            //
            //
            // looping over concerned controllers
            //
            //__________________________________________________________________
            FList  flist(bank);
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
                Y_XMLOG(xml, " (+) " << eq);

                const size_t   index = flist.size+1;               // new index
                const unsigned state = fence(cm,C0,xml);       // build the fence
                const unsigned flags = (state&Fence::ST_MASK); // get the flags

                //______________________________________________________________
                //
                // current controller may handle a negative concentration
                //______________________________________________________________
                switch( flags )
                {
                    case Fence::BLOCKED: goto CONTINUE;

                    case Fence::PARTIAL:
                        break;

                    case Fence::EQUATED:
                        break;

                    default:
                        throw Specific::Exception(here,"invalid flags='%s'", Fence::StatusText(flags) );

                }

                //______________________________________________________________
                //
                // compute and store improved state
                //______________________________________________________________
                assert(0!=(flags&Fence::IMPROVE));
                {
                    XWritable    &c = Ceqz[index];
                    const xreal_t g = cluster.equalized(c, SubLevel, eq, fence.cursor, fence.zeroed.head, C0, TopLevel, xadd);
                    const Fixed   f(g,c);
                    flist << f;
                }

            CONTINUE:;
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

