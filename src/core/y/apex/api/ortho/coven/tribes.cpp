#include "y/apex/api/ortho/coven/tribes.hpp"

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
#if 0
                void Tribes:: makeReplacement(XMLog &xml)
                {
                    assert( isSortedAccordingTo(Tribe::Compare) );
                    size_t replacement = 0;
                    for(Tribe *curr=head;curr;curr=curr->next)
                    {
                        assert(Foundation != curr->qfamily->quality );
                        for(Tribe *prev=curr->prev;prev;prev=prev->prev)
                        {
                            if( (curr->qfamily!=prev->qfamily) && (curr->content==prev->content) )
                            {
                                curr->adoptedBy(*prev);
                                ++replacement;
                            }
                        }
                    }

                    Y_XML_COMMENT(xml,"#replacement = " << replacement);
                    if(replacement<=0) { assert( isSortedAccordingTo(Tribe::Compare) ); return; }
                    MergeSort::Call(*this,Tribe::Compare);
                }
#endif

                void Tribes:: collect(Callback &proc)
                {
                    size_t & count = Coerce(collected);
                    count = 0;
                    for(const Tribe *tribe=head;tribe;tribe=tribe->next)
                    {
                        const Vector *pVector = tribe->lastVec;         if(0==pVector) continue;
                        const Vector *pSingle = tryInsertNew(*pVector); if(0==pSingle) continue;
                        proc(*pSingle);
                        ++count;
                    }
                }


            
              

            

#if 0
                void Tribes:: makeCompression(XMLog &xml)
                {
                    makeReplacement(xml);
                    size_t compression = 0;
                    {
                        Tribe::List kept;
                        while(size>0)
                        {
                            AutoPtr<Tribe> lhs = popHead();

                            for(Tribe *rhs=kept.head;rhs;rhs=rhs->next)
                            {
                                if(lhs->qfamily==rhs->qfamily) continue;

                                if(lhs->qfamily->isIdenticalTo(*rhs->qfamily))
                                {
                                    //std::cerr << "---- Same Vectors, different families" << std::endl;
                                    //std::cerr << "(*) lhs=" << *lhs << std::endl;
                                    //std::cerr << "(*) rhs=" << *rhs << std::endl;

                                    Posture &L = *lhs;
                                    Posture &R = *rhs;

                                    collapse(L,R);
                                    //std::cerr << "--> lhs=" << *lhs << std::endl;
                                    //std::cerr << "--> rhs=" << *rhs << std::endl;

                                    if(L==R)
                                    {
                                        //(std::cerr << '#').flush();
                                        ++compression;
                                        continue; // will drop lhs
                                    }
                                    
                                    throw Exception("Not Handled!!");
                                }
                            }

                            kept.pushTail(lhs.yield());
                        }

                        swapWith(kept);
                    }

                    Y_XML_COMMENT(xml,"#compression = " << compression);

                }
#endif

#if 0
                void Tribes:: removeFutile(XMLog &xml, const unsigned flag)
                {
                    Y_XML_COMMENT(xml, "#generated   = " << size);
                    Y_XML_COMMENT(xml, "#collected   = " << collected);

                    {
                        Tribe::List active;
                        while(size>0)
                        {
                            Tribe * const tribe = popHead();
                            if(Foundation == tribe->qfamily->quality || tribe->residue.size<=0)
                            {
                                delete tribe;
                                continue;
                            }
                            active.pushTail(tribe);
                        }
                        swapWith(active);
                        assert(isSortedAccordingTo(Tribe::Compare));
                    }



                    if(size<=0)
                        MergeSort::Call( Coerce(db), CompareVectors);
                    else
                    {
                        if( 0 != (flag & Tribe::UseBasisCompression ) )
                            makeCompression(xml);
                    }
                }
#endif

            }
        }

    }
}


