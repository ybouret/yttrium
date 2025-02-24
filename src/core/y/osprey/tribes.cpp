
#include "y/osprey/tribes.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Osprey
    {

        Apex::Natural Tribes:: MaxCount(const size_t n)
        {
            Apex::Natural res=0;
            for(size_t i=1;i<=n;++i) res += Apex::Natural::Arrange(n,i);
            return res;
        }

        void Tribes:: Display(const QVector &v)
        {
            std::cerr << "\t(+) " << v << std::endl;
        }

        void Tribes:: NoNullVec(const size_t zid, Tribe::List &tribes) noexcept
        {
            for(Tribe *tr=tribes.head;tr;tr=tr->next)
            {
                tr->posture.residue.removeNull(zid);
            }
        }

        void Tribes:: NoReplica(const size_t indx, Tribe::List &tribes)
        {
            for(Tribe *tr=tribes.head;tr;tr=tr->next)
            {
                if(!tr->posture.tryRemove(indx)) throw Specific::Exception("Tribes::NoReplica","missing index %u", unsigned(indx));
            }
        }


        Tribes:: ~Tribes() noexcept
        { while(db.size>0) vc->store( Coerce(db).popTail() ); }


        std::ostream & operator<<(std::ostream &os, const Tribes &self)
        {
            if(self.my.size<=0) os << "{}";
            else
            {
                os << "{ #" << self.my.size << std::endl;
                for(const Tribe *tr=self->head;tr;tr=tr->next)
                {
                    os << *tr << std::endl;
                }
                os << "}";

            }
            return os;
        }


        void Tribes:: noNullVec(XMLog &xml) noexcept
        {
            Tribe::List ok;
            while(my.size>0)
            {
                Tribe *tr = my.popHead();
                if(0==tr->lastVec)
                {
                    const size_t zid = tr->lastIdx;
                    Y_XML_COMMENT(xml, "removing null     data[" << zid << "]");
                    NoNullVec(zid,ok);
                    NoNullVec(zid,my);
                    delete tr;
                    continue;
                }
                ok.pushTail(tr);
            }
            my.swapWith(ok);
        }

        void Tribes:: noReplica(XMLog &xml, Callback &proc)
        {
            Tribe::List primary;
            while(my.size>0)
            {
                assert(0!=my.head->lastVec);
                const QVector *vec = tryInsert(*(my.head->lastVec));
                if(0!=vec)
                {
                    proc(*vec);
                    primary.pushTail( my.popHead() );
                }
                else
                {
                    const size_t indx = my.head->lastIdx;
                    delete my.popHead();
                    Y_XML_COMMENT(xml, "removing parallel data[" << indx << "]");
                    NoReplica(indx,primary);
                    NoReplica(indx,my);
                }
            }
            my.swapWith(primary);
        }


        Y_PROXY_IMPL(Tribes,my)


        void Tribes:: research(XMLog &xml, Callback &proc, const unsigned flag)
        {
            const bool useBasisCompression = 0 != (flag & Tribe::UseBasisCompression);
            size_t compressed = 0;
            for(Tribe *tribe=my.head;tribe;tribe=tribe->next)
            {
                // check if a new vector was created
                {
                    const QVector * const src = tribe->lastVec;
                    if(0!=src)
                    {
                        const QVector * const tgt = tryInsert(*src);
                        if(0!=tgt) proc(*tgt);
                    }
                }

                // check if possible basis compression
                if(useBasisCompression)
                {
                    for(const Tribe *guess=tribe->prev;guess;guess=guess->prev)
                    {
                        if( IList::AreEqual( *(tribe->posture.content),*(guess->posture.content) ) )
                        {
                            //std::cerr << "\tshould compress..." << std::endl;
                            ++compressed;
                            break;
                        }
                    }
                }
            }

            if(useBasisCompression)
            {
                Y_XML_COMMENT(xml, "#compressed = " << compressed);
            }

        }
    }

}

#include "y/hashing/function.hpp"
#include "y/stream/hash/output.hpp"
#include "y/hashing/md.hpp"

namespace Yttrium
{
    namespace Osprey
    {

        Digest Tribes:: signature(Hashing::Function &h) const
        {
            h.set();
            {
                HashingStream fp(h);
                for(const QVector *v=db.head;v;v=v->next)
                {
                    v->serialize(fp);
                }
            }
            
            return Hashing::MD::Of(h);
        }

    }

}
