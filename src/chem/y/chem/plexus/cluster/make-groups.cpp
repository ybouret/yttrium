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

                for(const EqNode *node=repo.head;node;node=node->next)
                {
                    if(xml.verbose)
                    {
                        const Equilibrium &eq = **node;
                        eqfm.pad( xml() << eq,eq) << " : " << conn[eq.indx[levl]] << std::endl;
                    }
                }
            }


            static inline bool AreConnected(const Equilibrium &lhs, const Equilibrium &rhs) noexcept
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

         }

        void Cluster:: makeGroups(XMLog &xml)
        {
            Y_XML_SECTION(xml, "Cluster::MakeGroups");


            {
                const size_t N = all.size; assert(all.size>0);
                Y_XML_SECTION_OPT(xml, "Solving", " count='" << N << "'");
                Matrix<bool> conn;
                BuildConn(conn,all, SubLevel, AreConnected);
                PrintConn(conn,all, SubLevel, eqfm, xml);

            }

            {
                const size_t NJ = army->definite.size;
                Y_XML_SECTION_OPT(xml, "Janitor", " count='" << NJ << "'");
                if(NJ>0)
                {
                    const AreDependent areDependent = { *kept };
                    Matrix<bool> conn;
                    BuildConn(conn,army->definite, AuxLevel, areDependent);
                    PrintConn(conn,army->definite, AuxLevel, eqfm, xml);
                }
            }

        }

    }

}
