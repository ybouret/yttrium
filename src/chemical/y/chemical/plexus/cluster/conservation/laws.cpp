
#include "y/chemical/plexus/cluster/conservation/laws.hpp"



namespace Yttrium
{
    namespace Chemical
    {
        namespace Conservation
        {

            Laws:: ~Laws() noexcept
            {
            }

            Laws:: Laws(const SList            &sp,
                        const Matrix<unsigned> &Qm) :
            Law::List(), 
            Assembly(),
            groups(),
            maxGroupSize(0)
            {
                //--------------------------------------------------------------
                //
                // create one law per conservation row
                //
                //--------------------------------------------------------------
                assert(Qm.cols==sp.size);
                for(size_t i=1;i<=Qm.rows;++i)
                    enroll( *pushTail( new Law(sp,Qm[i]) ) );


                //--------------------------------------------------------------
                //
                // create groups
                //
                //--------------------------------------------------------------
                {
                    Group::List &G = Coerce(groups);
                    for(const Law *law=head;law;law=law->next)
                    {
                        for(Group *g=G.head;g;g=g->next)
                        {
                            // look for accepting group
                            if(g->accept(*law))
                            {
                                (*g) << *law;
                                goto FUSION;
                            }
                        }

                        // start new group
                        G.pushTail( new Group(*law) );
                        continue;

                    FUSION:
                        Fusion(G);
                    }
                }

                //--------------------------------------------------------------
                //
                // compile groups
                //
                //--------------------------------------------------------------
                for(Group *g=groups.head;g;g=g->next)
                {
                    g->compile();
                    Coerce(maxGroupSize) = Max(g->size,maxGroupSize);
                }


            }

            void Laws:: Fusion(Group::List &G) noexcept
            {
                Group::List store;

                // check groups
                while(G.size>0)
                {
                    // extract first group
                    AutoPtr<Group> g = G.popHead();

                    // look for an accepting group
                    for(Group *lhs=store.head;lhs;lhs=lhs->next)
                    {
                        if(lhs->accept(*g))
                        {
                            lhs->mergeTail(*g);
                            g.erase();
                            break;
                        }
                    }

                    // is not accepted append to store
                    if(g.isValid()) store.pushTail( g.yield() );
                }

                // done
                G.swapWith(store);
            }



        }

    }
}

#include "y/sequence/vector.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Conservation
        {
            Laws:: Group:: ~Group() noexcept
            {
            }

            Laws:: Group:: Group(const Law &first) :
            Object(),
            LList(),
            crew(),
            next(0),
            prev(0)
            {
                (*this) << first;
            }

            bool Laws::Group:: accept(const Law &law) const noexcept
            {
                for(const LNode *ln=head;ln;ln=ln->next)
                {
                    if( law.linkedTo(**ln) ) return true;
                }
                return false;
            }

            bool Laws:: Group:: accept(const Group &group) const noexcept
            {
                for(const LNode *ln=group.head;ln;ln=ln->next)
                {
                    const Law &foreign = **ln;
                    if(accept(foreign)) return true;
                }
                return false;
            }

            void Laws:: Group:: compile()
            {
                //--------------------------------------------------------------
                //
                //
                // create crew of Species with AuxLevel
                //
                //
                //--------------------------------------------------------------
                {
                    AddressBook book;
                    for(const LNode *ln=head;ln;ln=ln->next) 
                    {
                        const Law &law = **ln;
                        law.record(book);
                    }
                    Indexed::AuxOrganize( book.sendTo( Coerce(crew) ) );
                }

                
                //--------------------------------------------------------------
                //
                //
                // update law
                //
                //
                //--------------------------------------------------------------
                const size_t     m = crew.size;
                Vector<xreal_t > alpha(m,0);
                for(LNode *ln=head;ln;ln=ln->next)
                {
                    Law     &law = Coerce(**ln);// std::cerr << law << std::endl;

                    //----------------------------------------------------------
                    //
                    // update projection matrix
                    //
                    //----------------------------------------------------------
                    {
                        XMatrix &mat = Coerce(law.proj);
                        mat.make(m,m);
                        alpha.ld(0);
                        for(const Actor *a=law->head;a;a=a->next)
                        {
                            alpha[ a->sp.indx[AuxLevel] ] = a->nu;
                        }
                        for(size_t i=m;i>0;--i)
                        {
                            for(size_t j=m;j>0;--j)
                            {
                                mat[i][j] = -alpha[i] * alpha[j];
                            }
                            mat[i][i] += law.xden;
                        }
                    }
                    
#if 0
                    //----------------------------------------------------------
                    //
                    // update species to keep
                    //
                    //----------------------------------------------------------
                    for(const SNode *sn=crew.head;sn;sn=sn->next)
                    {
                        const Species &sp = **sn;
                        if(law.hired(sp)) continue;
                        Coerce(law.keep) << sp;
                    }
#endif
                }

            }


        }
    }

}
