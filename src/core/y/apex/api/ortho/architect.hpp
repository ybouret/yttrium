
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

            //__________________________________________________________________
            //
            //
            //
            //! extract basis from row-major matrices
            //
            //
            //__________________________________________________________________
            class Architect : public Metrics
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef Small::SoloHeavyList<size_t> List; //!< alias
                typedef List::NodeType               Node; //!< alias

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit Architect(const size_t); //!< setup with metrics
                virtual ~Architect() noexcept;    //!< cleanup

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

                //! extract indices of the first rows of M that form a basis up to maxRank
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

                //! format target from extracted source
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

                //! clear all
                void clear() noexcept;

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

