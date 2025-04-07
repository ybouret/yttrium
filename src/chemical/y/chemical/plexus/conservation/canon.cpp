
#include "y/chemical/plexus/conservation/canon.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Conservation
        {


            Canon:: Canon(const Law &first) :
            LList(),
            rank(0),
            species(),
            anxious(),
            sformat(),
            next(0),
            prev(0)
            {
                (*this) << first;
            }

            Canon:: ~Canon() noexcept
            {
            }

            bool Canon:: accepts(const Law &law) const noexcept
            {
                for(const LNode *ln=head;ln;ln=ln->next)
                {
                    if( (**ln).hasCommonActorWith(law) ) return true;
                }
                return false;
            }


            bool Canon:: accepts(const Canon &rhs) const noexcept
            {
                for(const LNode *l=head;l;l=l->next)
                {
                    if(rhs.accepts(**l)) return true;
                }
                return false;
            }

        }

    }

}

#include "y/text/plural.hpp"
#include "y/mkl/algebra/rank.hpp"
#include "y/mkl/algebra/lu.hpp"
#include "y/counting/combination.hpp"
#include "y/system/exception.hpp"
#include "y/apex/api/simplify.hpp"

#include "y/associative/hash/map.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Conservation
        {
            using namespace MKL;

            class ProjectionKey : public Memory::ReadOnlyBuffer
            {
            public:
                typedef CxxArray<size_t,MemoryModel> ArrayType;

                ProjectionKey(const Readable<size_t> &sched) :
                arr(CopyOf,sched),
                ptr( &arr[1] ),
                len( arr.size() * sizeof(ArrayType::Type) )
                {
                }

                ProjectionKey(const ProjectionKey &pkey) :
                arr(pkey.arr),
                ptr( &arr[1] ),
                len( pkey.len )
                {

                }

                friend bool operator==(const ProjectionKey &lhs,
                                       const ProjectionKey &rhs) noexcept
                {
                    const size_t length = lhs.len;
                    if(length!=rhs.len) return false;
                    return 0 == memcmp(lhs.ptr, rhs.ptr, length);
                }

                virtual const void * ro_addr() const noexcept { return ptr; }
                virtual size_t       measure() const noexcept { return len; }

                virtual ~ProjectionKey() noexcept
                {
                }



            private:
                Y_DISABLE_ASSIGN(ProjectionKey);
                CxxArray<size_t,MemoryModel> arr;
                const void * const           ptr;
                const size_t                 len;
            };

            class ProjectionMatrix : public Object, public Counted, public iMatrix
            {
            public:
                typedef ArcPtr<const ProjectionMatrix> Pointer;

                explicit ProjectionMatrix(const Matrix<apz> &num,
                                          const apz         &den) :
                iMatrix(num.rows,num.cols),
                iDenom( den.cast<int>("projection denominator") ),
                xDenom( iDenom )
                {
                    iMatrix &self = *this;
                    for(size_t i=rows;i>0;--i)
                    {
                        for(size_t j=cols;j>0;--j)
                        {
                            self[i][j] = num[i][j].cast<Type>("projection numerator");
                        }
                    }
                }

                friend bool operator==(const ProjectionMatrix &lhs, const ProjectionMatrix &rhs) noexcept
                {
                    if( lhs.iDenom != rhs.iDenom || lhs.rows != rhs.rows || lhs.cols != rhs.cols) return false;
                    for(size_t i=lhs.rows;i>0;--i)
                    {
                        for(size_t j=lhs.cols;j>0;--j)
                        {
                            if( lhs[i][j] != rhs[i][j]) return false;
                        }
                    }
                    return true;
                }

                virtual ~ProjectionMatrix() noexcept
                {

                }

                const int     iDenom;
                const xreal_t xDenom;


            private:
                Y_DISABLE_COPY_AND_ASSIGN(ProjectionMatrix);
            };


            typedef HashMap<ProjectionKey,ProjectionMatrix::Pointer> ProjectionMap;

            void Canon:: compile(XMLog       & xml,
                                 const EList & definite)
            {
                Y_XML_SECTION_OPT(xml, "Conservation::Canon", "size=" << size);
                assert( 0 == maxNameLength );
                assert( 0 == species->size );

                //--------------------------------------------------------------
                //
                //
                // collect species and make AuxLevel
                //
                //
                //--------------------------------------------------------------
                Y_XML_COMMENT(xml, "law" << Plural::s(size));
                {
                    size_t auxId = 0;
                    for(const LNode *l=head;l;l=l->next)
                    {
                        const Law &law = **l; enroll( law );
                        Coerce(law.auxId) = ++auxId;
                        Y_XMLOG(xml,"(+) " <<law.name);
                        for(const Actor *a=law->head;a;a=a->next)
                        {
                            Coerce(species) << a->sp;
                            Coerce(sformat).enroll(a->sp);
                        }
                    }
                }



                Y_XML_COMMENT(xml, "species");;
                Y_XMLOG(xml, "(#) " << species);

                //--------------------------------------------------------------
                //
                //
                // compute the rank of the local matrix
                //
                //
                //--------------------------------------------------------------
                {
                    Y_XML_COMMENT(xml, "rank");
                    iMatrix    &iA = Coerce(iAlpha);
                    XMatrix    &xA = Coerce(xAlpha);
                    iA.make(size,species->size);
                    xA.make(size,species->size);
                    for(const LNode *ln=head;ln;ln=ln->next)
                    {
                        const Law &law = **ln;
                        law.iFillArray(iA[law.auxId], AuxLevel);
                        law.xFillArray(xA[law.auxId], AuxLevel);
                    }
                    Y_XMLOG(xml, "A   =" << iA);
                    Coerce(rank) = Rank::Of(iA);
                    Y_XMLOG(xml, "rank=" << rank);
                }

                //--------------------------------------------------------------
                //
                //
                // collect definite into anxious
                //
                //
                //--------------------------------------------------------------
                Y_XML_COMMENT(xml, "anxious");
                for(const ENode *en=definite.head;en;en=en->next)
                {
                    const Equilibrium &eq = **en;
                    if(eq.gotAnyOf(*species))
                    {
                        Coerce(anxious) << eq;
                        Y_XMLOG(xml, "(*) " << eq);
                    }
                }
                
                
            }

        }

    }

}
