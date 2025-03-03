#include "y/apex/api/ortho/coven/tribes.hpp"
#include "y/hashing/function.hpp"
#include "y/stream/hash/output.hpp"
#include "y/hashing/md.hpp"
namespace Yttrium
{
    namespace Apex
    {
        namespace Ortho
        {
            namespace Coven
            {
                const char * const Tribes:: CallSign = "Coven::Tribes";

                std::ostream & operator<<(std::ostream &os, const Tribes &self)
                {
                    os << '{' << '#' << self.size;
                    if(self.size>0)
                    {
                        os << std::endl;
                        for(const Tribe *tribe=self.head;tribe;tribe=tribe->next)
                        {
                            os << "  " << *tribe << std::endl;
                        }

                    }
                    os << '}';
                    return os;
                }


                Natural Tribes:: MaxCount(const size_t n)
                {
                    Natural res = 0;
                    for(size_t k=1;k<=n;++k) res += Natural::Arrange(n,k);
                    return res;
                }

                void Tribes:: Display(const Vector &v)
                {
                    std::cerr << "\t(+) " << v << std::endl;
                }

                Tribes:: ~Tribes() noexcept
                {
                    while(db.size) vc->store( Coerce(db).popTail() );
                }


             
                SignType Tribes:: CompareVectors(const Vector * const lhs,
                                                 const Vector * const rhs) noexcept
                {
                    return Vector::Compare(*lhs,*rhs);
                }

                Digest Tribes:: signature(Hashing::Function &H) const
                {
                    H.set();
                    {
                        HashingStream fp(H);
                        for(const Vector *v=db.head;v;v=v->next) v->serialize(fp);
                    }
                    return Hashing::MD::Of(H);
                }


                void Tribes:: collect(Callback &proc, uint64_t * const pEll)
                {
                    size_t & count = Coerce(collected);
                    count = 0;
                    const Vector *pSingle = 0;
                    for(const Tribe *tribe=head;tribe;tribe=tribe->next)
                    {
                        {
                            const StopWatch sw(pEll);
                            const Vector * const pVector = tribe->lastVec; if(0==pVector) continue;
                            pSingle = tryInsertNew(*pVector);
                            if(0==pSingle) continue;
                        }
                        proc(*pSingle);
                        ++count;
                    }
                }



            }
        }

    }
}


