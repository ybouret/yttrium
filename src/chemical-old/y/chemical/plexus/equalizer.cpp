#include "y/chemical/plexus/equalizer.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Equalizer:: Fixed:: ~Fixed() noexcept {}

        Equalizer:: Fixed:: Fixed(const xreal_t     theGain,
                                  const XReadable  &theConc,
                                  const Controller &theCntl,
                                  const Fence      &theWall) noexcept :
        gain(theGain),
        conc(theConc),
        cntl(theCntl),
        wall(theWall)
        {
        }

        Equalizer:: Fixed:: Fixed(const Fixed &other) noexcept :
        gain(other.gain),
        conc(other.conc),
        cntl(other.cntl),
        wall(other.wall)
        {
        }

        void Equalizer:: Fixed:: to(XMLog &xml, const char *pfx) const
        {
            if(!pfx) pfx = "";
            Y_XMLOG(xml, pfx << "gain = " << std::setw(15) << real_t(gain) << " @" << cntl.primary);
        }

        std::ostream &  Equalizer:: Fixed:: displayCompact( std::ostream &os ) const
        {
            cntl.primary.displayCompact(os,conc,SubLevel);
            return os;
        }

        void Equalizer:: Fixed:: set(XWritable &Ctop) const
        {
            cntl.primary.transfer(Ctop,TopLevel,conc,SubLevel);
        }

        void Equalizer:: Fixed:: add(XAddArray &xadds, XWritable &Ctop, AddressBook &rover) const
        {
            cntl.custom.transfer(Ctop,TopLevel,conc,SubLevel);
            for(const SNode *node=cntl.roving.head;node;node=node->next)
            {
                const Species &sp = **node;
                const size_t  si = sp.indx[SubLevel];
                const size_t  ti = sp.indx[TopLevel];
                const xreal_t dc = conc[si]-Ctop[ti]; // nu * xi
                rover     |= sp;
                xadds[si] << dc;
            }
        }

    }

}

namespace Yttrium
{
    namespace Chemical
    {

        Equalizer:: ~Equalizer() noexcept
        {
        }

        Equalizer:: Equalizer(const Clusters &clusters) :
        Ceqz(clusters.maxCPC,clusters.maxSPC),
        xadds(clusters.maxSPC),
        banks(),
        fences(clusters.maxCPC,banks),
        negative(),
        fbank(),
        flist(fbank),
        glist(fbank),
        rover(),
        xadd()
        {
        }


        size_t Equalizer:: getNegativeCntl(const XReadable &C0,
                                           const Cluster   &cluster)
        {
            const xreal_t zero;
            negative.free();
            for(const SNode *node=cluster.conservedSpecies.head;node;node=node->next)
            {
                const Species &sp = **node; assert( cluster.isLimited(sp) );
                if( C0[ sp.indx[TopLevel] ] < zero ) negative += sp;
            }
            return negative.size();
        }


        void Equalizer:: tuneControllers(XWritable     &C0,
                                         const Cluster &cluster,
                                         XMLog         &xml)

        {
            static const char here[] = "Chemical::Equalizer::tuneControllers";
            static const char ADD[]  = " (+) ";
            static const char BAD[]  = " (-) ";
            static const char WIN[]  = " ($) ";
            static const char AST[]  = " (*) ";

            //__________________________________________________________________
            //
            //
            // initialize
            //
            //__________________________________________________________________
            const SList  &species = cluster.conservedSpecies;
            if(species.size<=0) return;

            //__________________________________________________________________
            //
            //
            // probing negative conserved species ?
            //
            //__________________________________________________________________
            if( getNegativeCntl(C0, cluster)<=0 ) {
                Y_XMLOG(xml, AST << "no negative conserved");
                return;
            }




            //__________________________________________________________________
            //
            //
            // looping over concerned controllers
            //
            //__________________________________________________________________
            unsigned cycle = 0;
        CYCLE:
            ++cycle; assert(negative.size()>0);
            Y_XMLOG(xml, "-------- cycle = " << cycle << " --------");
            assert(negative.size()>0);
            if(xml.verbose) negative.display<Species>( xml() << BAD << "negative=") << std::endl;

            flist.free();
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

            //------------------------------------------------------------------
            //
            //
            // check fixed from controllers
            //
            //
            //------------------------------------------------------------------
            Y_XMLOG(xml, " (#) fixed=" << flist.size);
            if(flist.size<=0)
            {
                if(xml.verbose) negative.display<Species>( xml() << BAD << "negative=") << std::endl;
                return;
            }

            //------------------------------------------------------------------
            //
            //
            // sort by decreasing gain
            //
            //
            //------------------------------------------------------------------
            MergeSort::Call(flist,CompareFixed);
            if(xml.verbose)
            {
                for(const FNode *node=flist.head;node;node=node->next)
                {
                    (**node).to(xml,WIN);
                }
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
                const BMatrix &coop = cluster.cooperative;
                glist.free();
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
                Y_XML_SECTION_OPT(xml, "Improving"," count='" << glist.size << "'");
                cluster.spfmt.show(std::cerr, "ini@[", cluster.species, "]", C0, TopLevel, xml.depth);

                assert(glist.size>0);
                assert(0!=glist.head);


                const FNode *node = glist.head; assert(0!=node);
                const Fixed &first = **node;
                if(xml.verbose) first.displayCompact( xml() << AST ) << std::endl;
                if(glist.size<=1)
                {
                    //----------------------------------------------------------
                    // direct
                    //----------------------------------------------------------
                    first.set(C0);
                    cluster.spfmt.show(std::cerr, "end@[", cluster.species, "]", C0, TopLevel, xml.depth);

                }
                else
                {
                    //----------------------------------------------------------
                    // multiple
                    //----------------------------------------------------------
                    rover.free();
                    for(const SNode *sn=species.head;sn;sn=sn->next)
                    {
                        xadds[(**sn).indx[SubLevel]].free();
                    }

                    //----------------------------------------------------------
                    // add all roving and set all custom
                    //----------------------------------------------------------
                    first.add(xadds,C0,rover);
                    for(node=node->next;node;node=node->next)
                    {
                        const Fixed &f = **node;
                        if(xml.verbose) f.displayCompact( xml() << ADD ) << std::endl;
                        f.add(xadds,C0,rover);
                    }

                    if(xml.verbose) rover.display<Species>( xml() << ADD << "roving=") << std::endl;

                    //----------------------------------------------------------
                    // upgrading roving species
                    //----------------------------------------------------------
                    {
                        size_t nr = rover.size();
                        for(AddressBook::Iterator it = rover.begin();nr>0;--nr,++it)
                        {
                            const Species &      sp = *static_cast<const Species *>(*it);
                            const size_t * const id = sp.indx;
                            xreal_t             &c0 = C0[id[TopLevel]];
                            XAdd                &xa = xadds[id[SubLevel]];
                            xa << c0;
                            const xreal_t c0old = c0;
                            c0 = xa.sum();
                            if(xml.verbose) cluster.spfmt.pad( xml() << AST << sp, sp ) << " : " << real_t(c0old) << " -> " << real_t(c0) << std::endl;;
                        }
                    }
                }


            }

            if( getNegativeCntl(C0,cluster) )
            {
                goto CYCLE;
            }

            Y_XMLOG(xml, AST << "done");
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

