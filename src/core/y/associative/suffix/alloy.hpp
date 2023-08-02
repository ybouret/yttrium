
//! \file


#ifndef Y_Associative_Suffix_Alloy_Included
#define Y_Associative_Suffix_Alloy_Included 1

#include "y/associative/suffix/tree.hpp"
#include "y/associative/interface.hpp"

namespace Yttrium
{


    template <typename KEY, typename T, typename NODE, typename BASE>
    class SuffixAlloy :
    public Associative<KEY,T>,
    public SuffixTree<KEY,T,NODE>, public BASE
    {
    public:
        typedef SuffixTree<KEY,T,NODE> TreeType;
        using TreeType::list;
        using TreeType::pool;
        using TreeType::tree;

        virtual ~SuffixAlloy() noexcept {}

        inline virtual const char * callSign()  const noexcept { return BASE::CallSign;  }
        inline virtual size_t       size()      const noexcept { return list.size; }
        inline virtual size_t       capacity()  const noexcept { return list.size+pool.size; }
        inline virtual size_t       available() const noexcept { return pool.size; }
        inline virtual void         free()            noexcept { this->softReset(); }
        inline virtual void         release()         noexcept { this->hardReset(); }
        inline virtual void         reserve(size_t n) noexcept { pool.reserve(n);   }


    protected:
        explicit SuffixAlloy() noexcept :
        Associative<KEY,T>(),
        TreeType()
        {
        }


    private:
        Y_DISABLE_ASSIGN(SuffixAlloy);
    };
    

}

#endif

