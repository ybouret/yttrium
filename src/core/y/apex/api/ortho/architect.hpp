
//! \file

#ifndef Y_Apex_Ortho_Architect_Included
#define Y_Apex_Ortho_Architect_Included 1

#include "y/apex/api/ortho/family.hpp"
#include "y/data/small/heavy/list/solo.hpp"
#include "y/container/matrix.hpp"
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
            class Architect : public Metrics, public Proxy<const Family>
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
                    target.release();
                    const List &l = extract(source,maxRank);
                    if(l.size<=0) return;
                    target.make(l.size,dimensions);
                    size_t i=1;
                    for(const Node *node=l.head;node;node=node->next,++i)
                        target[i].ld( source[ **node ] );
                }

                //! format target with family vectors
                template <typename TARGET, typename SOURCE> inline
                void basis(TARGET &      target,
                           const SOURCE &source,
                           const size_t  maxRank)
                {
                    target.release();
                    const List &l = extract(source,maxRank);
                    if(l.size<=0) return;
                    target.make(l.size,dimensions);
                    size_t i=1;
                    for(const Vector *v=family->head;v;v=v->next,++i)
                    {
                        target[i].ld( *v );
                    }
                }


                //! format target columns with family of vectors
                template <typename TARGET, typename SOURCE> inline
                void transposeBasis(TARGET       &target,
                                    const SOURCE &source,
                                    const size_t  maxRank)
                {
                    target.release();
                    Matrix<apz> B; basis(B,source,maxRank);
                    target.make(B.cols,B.rows);

                    for(size_t j=B.cols;j>0;--j)
                    {
                        for(size_t i=B.rows;i>0;--i)
                            target[j][i] = B[i][j];
                    }

                }


                //! clear all
                void clear() noexcept;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Architect);
                Y_PROXY_DECL();
                
                void record(const size_t i);

                VCache vcache;
                Family family;
                List   ivList;
            };



        }

    }

}

#endif

