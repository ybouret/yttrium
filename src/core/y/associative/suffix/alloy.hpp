
//! \file


#ifndef Y_Associative_Suffix_Alloy_Included
#define Y_Associative_Suffix_Alloy_Included 1

#include "y/associative/suffix/tree.hpp"
#include "y/associative/interface.hpp"

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //
    //! Alloy from a SuffixTree
    //
    //
    //__________________________________________________________________________
    template <typename KEY, typename T, typename NODE, typename BASE>
    class SuffixAlloy :
    public Associative<KEY,T>,
    public SuffixTree<KEY,T,NODE>, public BASE
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        typedef SuffixTree<KEY,T,NODE> TreeType; //!< alias
        using TreeType::list;
        using TreeType::pool;
        using TreeType::tree;

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        inline virtual const char * callSign()  const noexcept { return BASE::CallSign; }      //!< callSign
        inline virtual size_t       size()      const noexcept { return list.size; }           //!< list size
        inline virtual size_t       capacity()  const noexcept { return list.size+pool.size; } //!< capacity
        inline virtual size_t       available() const noexcept { return pool.size; }           //!< available
        inline virtual void         free()            noexcept { this->softReset(); }          //!< free content, keep memory
        inline virtual void         release()         noexcept { this->hardReset(); }          //!< release all possible memory
        inline virtual void         reserve(size_t n) noexcept { pool.reserve(n); }            //!< reserve blocks

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________

        //! cleanup
        inline virtual ~SuffixAlloy() noexcept {}

    protected:

        //! setup empty
        inline explicit SuffixAlloy() noexcept :
        Associative<KEY,T>(),
        TreeType(),
        BASE()
        {
        }

        //! copy
        inline SuffixAlloy(const SuffixAlloy &other) noexcept :
        Identifiable(), Collection(),
        Associative<KEY,T>(),
        TreeType(other),
        BASE()
        {
        }




    private:
        Y_DISABLE_ASSIGN(SuffixAlloy);
    };
    

}

#endif

