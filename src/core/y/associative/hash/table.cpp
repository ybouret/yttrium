#include "y/associative/hash/table.hpp"
#include "y/object.hpp"
#include "y/memory/wad.hpp"
#include "y/memory/blanks.hpp"
#include "y/data/list.hpp"
#include "y/calculus/ilog2.hpp"
#include "y/calculus/base2.hpp"
#include "y/system/exception.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/type/utils.hpp"

namespace Yttrium
{
    HashKnot:: HashKnot(const size_t h, void *p) noexcept :
    next(0), prev(0), hkey(h), node(p)
    {
    }

    HashKnot:: ~HashKnot() noexcept
    {
    }


    namespace
    {

        class Metrics
        {
        public:
            typedef HashKnot::List Slot;

            static const unsigned MinSlotsLn2 = 4;
            static const size_t   MinSlots    = 1 << MinSlotsLn2;
            static const size_t   MaxSlots    = (Base2<size_t>::MaxPowerOfTwo / sizeof(Slot))/HashTable::LoadFactor;

            static inline size_t CheckSize(const size_t n)
            {
                if(n>MaxSlots) throw Exception("too big");
                return Max(MinSlots,NextPowerOfTwo(n));
            }

            inline explicit Metrics(const size_t sz) :
            size(  CheckSize(sz) ),
            mask( size-1 )
            {
                assert(size>=MinSlots);
            }

            inline virtual ~Metrics() noexcept
            {
            }

            const size_t size;
            const size_t mask;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Metrics);
        };



    }

    class HashTable::Code :
    public Object,
    public Metrics,
    public Memory::Wad<Metrics::Slot,Memory::Dyadic>
    {
    public:

        typedef Memory::Wad<Slot,Memory::Dyadic> WadType;


        inline explicit Code(const size_t sz ) :
        Metrics(sz),
        WadType(size),
        slot( static_cast<Slot *>(workspace) )
        { setup(); }


        inline virtual ~Code() noexcept { clear(); }

        inline void steal(Code *source) noexcept
        {
            assert(0!=source);
            for(size_t i=0;i<source->size;++i)
            {
                Slot &src = source->slot[i];
                while(src.size) {
                    HashKnot    *knot = src.popHead();
                    const size_t hkey = knot->hkey;
                    slot[hkey&mask].pushTail(knot);
                }
            }

        }

        inline void freeWith(HashKnot::Pool &pool) noexcept
        {
            for(size_t i=0;i<size;++i)
            {
                Slot &s =  slot[i];
                while(s.size>0) pool.storeBuilt(s.popTail());
            }
        }

        inline void releaseWith(HashKnot::Pool &pool) noexcept
        {
            for(size_t i=0;i<size;++i)
            {
                Slot &s =  slot[i];
                while(s.size>0) pool.eraseBuilt(s.popTail());
            }
        }

        Slot        *slot;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Code);
        inline void setup() noexcept { for(size_t i=0;i<size;++i) new (slot+i) Slot(); }
        inline void clear() noexcept { for(size_t i=0;i<size;++i) Destruct(slot+i);    }
    };


    HashTable:: HashTable(const size_t sz) : code( new Code(sz) )
    {
        
    }

    HashTable:: ~HashTable() noexcept
    {
        assert(0!=code);
        delete code;
        code = 0;
    }


    
    HashKnot::List & HashTable:: operator[](const size_t hkey) noexcept
    {
        assert(0!=code);
        assert(0!=code->slot);
        return code->slot[hkey&code->mask];
    }

    const HashKnot::List & HashTable:: operator[](const size_t hkey) const noexcept
    {
        assert(0!=code);
        assert(0!=code->slot);
        return code->slot[hkey&code->mask];
    }


    size_t HashTable:: size() const noexcept
    {
        assert(code!=0);
        return code->size;
    }

    void HashTable:: grow()
    {
        assert(code!=0);
        const size_t curr = code->size;
        if(curr>=Code::MaxSlots) Exception("Already got maximum slots");

        Code *temp = new Code(curr*2);
        temp->steal(code);
        delete code;
        code = temp;
    }

    void HashTable:: freeWith(HashKnot::Pool &pool) noexcept
    {
        assert(code!=0);
        code->freeWith(pool);
    }

    void HashTable:: releaseWith(HashKnot::Pool &pool) noexcept
    {
        assert(code!=0);
        code->releaseWith(pool);
    }



};
