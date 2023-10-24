#include "y/chem/plexus/cluster.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace
        {

            template <typename PROC> static inline
            void BuildConn(Matrix<bool> &conn,
                           const EqRepo &repo,
                           const Level   level,
                           PROC         &connected)
            {
                assert(0==conn.rows);
                assert(repo.size>0);
                const size_t n = repo.size;
                conn.make(n,n);

                for(const EqNode *lhs=repo.head;lhs;lhs=lhs->next)
                {
                    const Equilibrium &l  = **lhs;
                    const size_t       il = l.indx[level];
                    for(const EqNode *rhs=lhs;rhs;rhs=rhs->next)
                    {
                        const Equilibrium &r  = **rhs;
                        const size_t       ir = r.indx[level];
                        conn[ir][il] = conn[il][ir] = connected(l,r);
                    }
                }
            }

            static inline void PrintConn(const Matrix<bool> &conn,
                                        const EqRepo       &repo,
                                        const Level         levl,
                                        const Entities     &eqfm,
                                        XMLog &xml)
            {

                return;

                for(const EqNode *node=repo.head;node;node=node->next)
                {
                    if(xml.verbose)
                    {
                        const Equilibrium &eq = **node;
                        eqfm.pad( xml() << eq,eq) << " : " << conn[eq.indx[levl]] << std::endl;
                    }
                }
            }


            static inline bool areConnected(const Equilibrium &lhs, const Equilibrium &rhs) noexcept
            {
                return lhs.linkedTo(rhs);
            }

            struct AreDependent
            {
                const Booleans &kept;

                inline bool operator()(const Equilibrium &lhs, const Equilibrium &rhs) const noexcept
                {
                    if( &lhs == &rhs ) return true;

                    for(Equilibrium::ConstIterator lit=lhs->begin();lit!=lhs->end();++lit)
                    {
                        const Species &ls = (*lit).sp;
                        for(Equilibrium::ConstIterator rit=rhs->begin();rit!=rhs->end();++rit)
                        {
                            const Species &rs = (*rit).sp;

                            if( &ls == &rs )
                            {
                                if(kept[ls.indx[SubLevel]])
                                {
                                    //std::cerr << "common kept " << ls << " in " << lhs << " and " << rhs << std::endl;
                                    return true;
                                }
                            }

                        }
                    }
                    return false;
                }
            };


            struct CompareGroups
            {
                const Level level;

                inline SignType operator()(const EqGroup *lhs, const EqGroup *rhs) noexcept
                {
                    assert(0!=lhs);
                    assert(0!=rhs);
                    switch(Sign::Of(lhs->size,rhs->size))
                    {
                        case Negative: return Negative;
                        case Positive: return Positive;
                        case __Zero__: break;
                    }

                    assert(lhs->head);
                    assert(rhs->head);
                    const Equilibrium &L = **(lhs->head);
                    const Equilibrium &R = **(rhs->head);
                    return Sign::Of(L.indx[level],R.indx[level]);
                }
            };



            inline
            void SetGroups(const Matrix<bool> &conn,
                           const EqRepo       &repo,
                           const Level         levl,
                           EqGroups           &grps,
                           XMLog              &xml)
            {
                //--------------------------------------------------------------
                //
                //
                // initialize with each single
                //
                //
                //--------------------------------------------------------------
                for(const EqNode *node=repo.head;node;node=node->next)
                    grps.pushTail( new EqGroup(**node) );

                //--------------------------------------------------------------
                //
                //
                // try to add each single to previous group
                //
                //
                //--------------------------------------------------------------
                for(const EqNode *node=repo.head;node;node=node->next)
                {
                    //----------------------------------------------------------
                    //
                    // get single
                    //
                    //----------------------------------------------------------
                    const Equilibrium &lhs = **node;
                    const size_t       lid = lhs.indx[levl];

                    //----------------------------------------------------------
                    //
                    // loop over previous group
                    //
                    //----------------------------------------------------------
                    for(EqGroup *egrp=grps.head;egrp;egrp=egrp->next)
                    {
                        //------------------------------------------------------
                        // strictly increasing index, avoid repeat
                        //------------------------------------------------------
                        assert(egrp->size>0);
                        if(lid<= (**(egrp->tail)).indx[levl]) continue;

                        //------------------------------------------------------
                        // check not connected
                        //------------------------------------------------------
                        const Readable<bool> &flag      = conn[lid];
                        bool                  connected = false;
                        for(const EqNode *scan=egrp->head;scan;scan=scan->next)
                        {
                            const Equilibrium &rhs = **scan;
                            const size_t       rid = rhs.indx[levl];
                            if(flag[rid])
                            {
                                connected = true;
                                break;
                            }
                        }
                        if(connected) continue;

                        //------------------------------------------------------
                        // duplicate at head for next pass
                        //------------------------------------------------------
                        (*grps.pushHead( new EqGroup(*egrp) )) << lhs;
                    }
                }

                //--------------------------------------------------------------
                //
                //
                // Order by size and first index
                //
                //
                //--------------------------------------------------------------
                CompareGroups cmp = { levl };
                MergeSort::Call(grps,cmp);
                const size_t origin = repo.size;
                const size_t total  = grps.size;
                const size_t extra  = total-origin;
                Y_XMLOG(xml, "  |- #Count : " << origin);
                Y_XMLOG(xml, "  |- #Total : " << total);
                Y_XMLOG(xml, "  |- #Extra : " << extra);

                for(EqGroup *g=grps.head;g;g=g->next)
                {
                    if(g->size>1)
                    {
                        Y_XMLOG(xml," (*) " << *g);
                    }
                }
            }

         }

        void Cluster:: makeGroups(XMLog &xml)
        {
            Y_XML_SECTION(xml, "Cluster::MakeGroups");


            {
                const size_t N = all.size; assert(all.size>0);
                Y_XML_SECTION_OPT(xml, "Solving", " count='" << N << "'");
                Matrix<bool> conn;
                BuildConn(conn,all, SubLevel, areConnected);
                PrintConn(conn,all, SubLevel, eqfm, xml);
                SetGroups(conn,all, SubLevel, Coerce(*sGrp), xml );
            }

            {
                const size_t NJ= army->definite.size;
                Y_XML_SECTION_OPT(xml, "Equalizer", " count='" << NJ << "'");
                if(NJ>0)
                {
                    const AreDependent areDependent = { *kept };
                    Matrix<bool> conn;
                    BuildConn(conn,army->definite, AuxLevel, areDependent);
                    PrintConn(conn,army->definite, AuxLevel, eqfm, xml);
                    SetGroups(conn,army->definite, AuxLevel, Coerce(*eGrp), xml );
                }
            }

        }

    }

}
