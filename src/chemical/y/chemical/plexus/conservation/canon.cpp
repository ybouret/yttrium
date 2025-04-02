
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
                    Matrix<apq> Q(size,species->size);
                    {
                        size_t i=1;
                        for(const LNode *ln=head;ln;ln=ln->next,++i)
                        {
                            const Law &law = **ln;
                            law.fillArray(Q[i],AuxLevel);
                        }
                    }
                    Y_XMLOG(xml, "Q   =" << Q);
                    Coerce(rank) = Rank::Compute(Q);
                    Y_XMLOG(xml, "rank=" << rank);
                }

                return;

                //--------------------------------------------------------------
                //
                //
                // compute all possibilities
                //
                //
                //--------------------------------------------------------------
                LU<apq>      lu(rank);
                const size_t n = size;
                const size_t m = species->size;
                const apq    q0;

                for(size_t k=1;k<=rank;++k)
                {
                    Matrix<apz> A(k,m);
                    Matrix<apq> A2(k,k);
                    Matrix<apq> Q2(k,k); // to test rank
                    Matrix<apq> adjA2(k,k);
                    Matrix<apz> A3(k,m); // adjA2 * A
                    Matrix<apz> A4(m,m); // A' * A3

                    LightArray<apz> tab = A4.asArray();

                    ProjectionMap pmap;

                    size_t      nmat = 0;
                    size_t      ndif = 0;
                    Combination comb(n,k);
                    Y_XML_SECTION_OPT(xml,"Projection", "rank=" << k << " maxi=" << comb.total);

                    do
                    {
                        A.ld(q0);
                        for(size_t i=1;i<=k;++i)
                        {
                            const Law &law = **fetch( comb[i] );
                            law.fillArray(A[i],AuxLevel);
                        }

                        for(size_t i=k;i>0;--i)
                        {
                            const Readable<apz> &Ai = A[i];
                            for(size_t j=i;j>0;--j)
                            {
                                const Readable<apz> &Aj = A[j];
                                apz sum = 0; for(size_t l=m;l>0;--l) sum += Ai[l] * Aj[l];
                                A2[i][j] = A2[j][i] = sum;
                            }
                        }

                        Q2.assign(A2);
                        if(k!=Rank::Compute(Q2))
                        {
                            std::cerr << "// drop " << comb << std::endl;
                            continue;
                        }
                        ++nmat;

                        //std::cerr << "A="  << A  << std::endl;
                        //std::cerr << "A2=" << A2 << std::endl;
                        lu.adjoint(adjA2,A2);
                        if(!lu.build(A2)) throw Specific::Exception("Chemical::Canon","corrupted Gram Matrix!!");
                        const apz detA2 = lu.determinant(A2);
                        //std::cerr << "adjA2=" << adjA2 << std::endl;
                        //std::cerr << "detA2=" << detA2 << std::endl;

                        for(size_t i=k;i>0;--i)
                        {
                            for(size_t j=m;j>0;--j)
                            {
                                apz sum = 0;
                                for(size_t l=k;l>0;--l)
                                {
                                    sum += adjA2[i][l].numer * A[l][j];
                                }
                                A3[i][j] = sum;
                            }
                        }
                        //std::cerr << "A3=" << A3 << std::endl;

                        for(size_t i=m;i>0;--i)
                        {
                            for(size_t j=i;j>0;--j)
                            {
                                apz sum = 0;
                                for(size_t l=k;l>0;--l)
                                {
                                    sum -= A[l][i] * A3[l][j];
                                }
                                if(i==j) sum += detA2;
                                A4[i][j] = A4[j][i] = sum;
                            }
                        }
                        Apex::Simplify::Array(tab,detA2);
                        std::cerr << "A4=" << A4 << "/" << detA2 << " //" << comb << std::endl;

                        const ProjectionMatrix::Pointer lhs( new ProjectionMatrix(A4,detA2));
                        const ProjectionKey             key = comb;
                        //std::cerr << comb << "=>"; key.displayHexadecimal(std::cerr) << std::endl;

                        bool found = false;
                        for(ProjectionMap::Iterator it=pmap.begin();it!=pmap.end();++it)
                        {
                            const ProjectionMatrix::Pointer &rhs = *it;
                            if( *lhs == *rhs )
                            {
                                found = true;
                                if(!pmap.insert(key,rhs)) throw Exception("couldn't insert");
                                break;
                            }
                        }

                        if(!found)
                        {
                            if(!pmap.insert(key,lhs)) throw Exception("couldn't insert");
                            ++ndif;
                        }






                    } while( comb.next() );
                    Y_XML_COMMENT(xml, "found " << nmat << " / " << comb.total << " ==> " << ndif );
                    //Y_XML_COMMENT(xml, "sizeof ProjectionMatrix = " << sizeof(ProjectionMatrix) );
                    //Y_XML_COMMENT(xml, "sizeof ProjectionKey    = " << sizeof(ProjectionKey) );



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
