#include "y/associative/hash/table.hpp"
#include "y/object.hpp"
#include "y/memory/embedded.hpp"
#include "y/data/list.hpp"
#include "y/data/pool.hpp"
#include "y/check/static.hpp"
#include "y/calculus/ilog2.hpp"
#include "y/calculus/base2.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace
    {
        class HashTableKnot
        {
        public:
            inline HashTableKnot(const size_t h, void *p) noexcept :
            next(0), prev(0), hkey(h), node(p)
            {
            }

            inline ~HashTableKnot() noexcept
            {
            }


            HashTableKnot *next;
            HashTableKnot *prev;
            const size_t   hkey;
            void          *node;
        private:
            Y_DISABLE_COPY_AND_ASSIGN(HashTableKnot);
        };


        typedef ListOf<HashTableKnot> HashTableSlot;
        typedef PoolOf<HashTableKnot> HashTablePool;



    }

    class HashTable::Code : public Object
    {
    public:
        typedef HashTableKnot Knot;
        typedef HashTableSlot Slot;
        static  const size_t  KnotSizeShift  = iLog2Of<Knot>::Value;
        static  const size_t  SlotSizeShift  = iLog2Of<Slot>::Value;
        static  const size_t  MaxCapacityLn2 = Base2<size_t>::MaxShift - (KnotSizeShift+SlotSizeShift);
        static  const size_t  MaxCapacity    = Base2<size_t>::One << MaxCapacityLn2;

        explicit Code(const size_t n)
        {
        }

        virtual ~Code() noexcept
        {
        }

        Slot        *slot;
        const size_t size;
        const size_t mask;
        Pool         pool;
        
    private:

    };

    HashTable:: HashTable(const size_t n) : code( 0 )
    {
        Y_STATIC_CHECK(iLog2Of<HashTableKnot>::Value == iLog2Of<HashTableSlot>::Value,BadSizeOf);
        std::cerr << "sizeof(HashTableKnot)=" << sizeof(HashTableKnot) << std::endl;
        std::cerr << "sizeof(HashTableSlot)=" << sizeof(HashTableKnot) << std::endl;
        std::cerr << "MaxiCapaShift = " << Code::MaxiCapaShift << std::endl;

        if(n>Code::MaxCapacity) throw Exception("too many");
        n = NextPowerOfTwo(n);
        Memory::Embed emb[] =
        {
        }

    }

    HashTable:: ~HashTable() noexcept
    {
        assert(0!=code);
        delete code;
        code = 0;
    }




};
