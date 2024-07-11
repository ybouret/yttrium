
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
                // find max group size
                //
                //--------------------------------------------------------------
                for(const Group *g=groups.head;g;g=g->next)
                    Coerce(maxGroupSize) = Max(g->size,maxGroupSize);


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


        }
    }

}
