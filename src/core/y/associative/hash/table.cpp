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
    HashTableKnot:: HashTableKnot(const size_t h, void *p) noexcept :
    next(0), prev(0), hkey(h), node(p)
    {
    }

    HashTableKnot:: ~HashTableKnot() noexcept
    {
    }


    namespace
    {



        typedef ListOf<HashTableKnot> HashTableSlot;



        class Metrics
        {
        public:
            typedef HashTableKnot Knot;
            typedef HashTableSlot Slot;

            static const size_t   MinSlots   = 16;
            static const size_t   LoadFactor = 4;
            static const size_t   MaxItems   = Base2<size_t>::MaxPowerOfTwo;
            static const size_t   MinItems   = MinSlots * LoadFactor;

            static inline size_t CheckItems(const size_t n)
            {
                if(n>MaxItems) throw Exception("Too many items");
                return Max(MinItems,NextPowerOfTwo(n));
            }

            inline explicit Metrics(const size_t n) :
            size( CheckItems(n)/LoadFactor ),
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
    public Memory::Wad<HashTableSlot,Memory::Dyadic>
    {
    public:

        typedef Memory::Wad<Slot,Memory::Dyadic> WadType;



        explicit Code(const size_t n) :
        Metrics(n),
        WadType(size),
        slot( static_cast<Slot *>(workspace) )
        {
            for(size_t i=0;i<size;++i) new (slot+i) Slot();
        }

        virtual ~Code() noexcept
        {
        }

        Slot        *slot;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Code);

    };

    HashTable:: HashTable(const size_t n) : code( new Code(n) )
    {


    }

    HashTable:: ~HashTable() noexcept
    {
        assert(0!=code);
        delete code;
        code = 0;
    }


    
    void HashTable:: insert(HashTableKnot *knot) noexcept
    {
        assert(0!=code);
        assert(0!=knot);
        assert(0!=code->slot);
        code->slot[knot->hkey&code->mask].pushHead(knot);
    }

    size_t HashTable:: size() const noexcept
    {
        assert(code!=0);
        return code->size;
    }

};
