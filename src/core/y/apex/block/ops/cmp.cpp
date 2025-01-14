#include "y/apex/block.hpp"




namespace Yttrium
{
    namespace Apex
    {
        bool operator==(const Block &lhs, const Block &rhs) noexcept
        {
            if(lhs.bits!=rhs.bits) return false;

            const Block::Briefly _lhs(Plan8,lhs);
            const Block::Briefly _rhs(Plan8,rhs);
            const Jig8 &l = lhs.as<Plan8>();
            const Jig8 &r = rhs.as<Plan8>();
            assert(l.words == r.words);
            const Jig8::Word *L = l.word;
            const Jig8::Word *R = r.word;
            
            for(size_t i=l.words;i>0;--i) {
                if( *(L++) != *(R++) ) return false;
            }
            return true;
        }
    }

}

