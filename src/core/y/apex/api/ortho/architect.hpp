
//! \file

#ifndef Y_Apex_Ortho_Architect_Included
#define Y_Apex_Ortho_Architect_Included 1

#include "y/apex/api/ortho/family.hpp"
#include "y/data/small/heavy/list/solo.hpp"
#include "y/type/utils.hpp"
#include "y/exception.hpp"

namespace Yttrium
{
    namespace Apex
    {

        namespace Ortho
        {



            class Architect : public Metrics
            {
            public:
                typedef Small::SoloHeavyList<size_t> List;
                typedef List::NodeType               Node;

                explicit Architect(const size_t);
                virtual ~Architect() noexcept;

                template <typename MATRIX> inline
                const List & extract(const MATRIX &M, const size_t maxRank)
                {
                    assert(dimensions==M.cols);
                    clear();
                    const size_t n = M.rows;
                    const size_t d = Min(maxRank,dimensions);


                    for(size_t i=1;i<=n;++i)
                    {
                        if(family->size>=d)        break;     // check not done already
                        if(!family.welcomes(M[i])) continue;  // check independent vector
                        record(i);
                    }

                    return ivList;
                }

                template <typename TARGET, typename SOURCE> inline
                void extract(TARGET &      target,
                             const SOURCE &source,
                             const size_t  maxRank)
                {
                    const List &l = extract(source,maxRank);
                    if(l.size<=0) { target.release(); return; }
                    target.make(l.size,dimensions);
                    size_t i=1;
                    for(const Node *node=l.head;node;node=node->next,++i)
                        target[i].ld( source[ **node ] );
                }


                void clear() noexcept { family.reset(); ivList.free(); }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Architect);
                void record(const size_t i);

                VCache vcache;
                Family family;
                List   ivList;
            };


        }

    }

}

#endif

