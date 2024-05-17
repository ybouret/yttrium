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
        fbank(),
        flist(fbank),
        glist(fbank),
        xadd()
        {
        }


        

        void Equalizer:: tuneControllers(XWritable     &C0,
                                         const Cluster &cluster,
                                         XMLog         &xml)

        {
            static const char here[] = "Chemical::Equalizer::tuneControllers";
            static const char ADD[]  = " (+) ";
            static const char BAD[]  = " (-) ";
            static const char WIN[]  = " ($) ";

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
            const BMatrix &coop = cluster.cooperative;
        CYCLE:
            flist.free();
            glist.free();
            for(const Controller *cntl=cluster.controllers.head;cntl;cntl=cntl->next)
            {
                const Equilibrium &eq = cntl->primary;
                const Components  &cm = cntl->custom;
                if( ! cm.isConcernedBy(negative) )
                {
                    Y_XMLOG(xml,BAD << eq);
                    continue;
                }

                //______________________________________________________________
                //
                // current controller may handle a negative concentration
                //______________________________________________________________
                if(xml.verbose) cluster.eqfmt.print( xml() << ADD << eq << " : ", eq) << std::endl;


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
                        xml() << WIN << real_t(g1) << std::endl;
                    }
                }

            CONTINUE:;
            }

            Y_XMLOG(xml, " (#) fixed=" << flist.size);
            if(flist.size<=0)
                return;

            //------------------------------------------------------------------
            //
            //
            // sort by decreasing gain
            //
            //
            //------------------------------------------------------------------
            MergeSort::Call(flist,CompareFixed);
            for(const FNode *node=flist.head;node;node=node->next)
            {
                (**node).to(xml,WIN);
            }

            //------------------------------------------------------------------
            //
            //
            // select cooperative solution
            //
            //
            //------------------------------------------------------------------
            {
                Y_XML_SECTION(xml, "Selecting");
               
                //--------------------------------------------------------------
                //
                // select first one
                //
                //--------------------------------------------------------------
                glist.pushTail( flist.popHead() );
                (**glist.head).to(xml, ADD);

                //--------------------------------------------------------------
                //
                // append cooperating with glist
                //
                //--------------------------------------------------------------
                while( flist.size > 0 )
                {
                    const Fixed          &rhs        = **flist.head;
                    const Readable<bool> &flag       = coop[*rhs.cntl];
                    bool                  cooperates = true;
                    for(const FNode *node=glist.head;node;node=node->next)
                    {
                        const Fixed &lhs = **node;
                        const bool   ok  = flag[ *lhs.cntl ];
                        if(!ok)
                        {
                            cooperates=false;
                            break;
                        }
                    }

                    if(cooperates)
                    {
                        rhs.to(xml,ADD);
                        glist.pushTail( flist.popHead() );
                    }
                    else
                    {
                        flist.cutHead();
                    }
                }
            }

            //------------------------------------------------------------------
            //
            //
            // build solution
            //
            //
            //------------------------------------------------------------------
            {
                Y_XML_SECTION(xml, "Improving");
                cluster.spfmt.show(std::cerr, "init@[", cluster.species, "]", C0, TopLevel);

                // assign first one
                const FNode *node = glist.head; assert(0!=node);
                {
                    const Fixed &f = **node;
                    if(xml.verbose) f.displayCompact( xml() << " (*) ") << std::endl;
                }

                for(node=node->next;node;node=node->next)
                {
                    const Fixed &f = **node;
                    if(xml.verbose) f.displayCompact( xml() << ADD ) << std::endl;
                }

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

