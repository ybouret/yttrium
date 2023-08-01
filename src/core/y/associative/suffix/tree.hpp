//! \file


#ifndef Y_Associative_Suffix_Tree_Included
#define Y_Associative_Suffix_Tree_Included 1

#include "y/string.hpp"

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //
    //! Anonymous SuffixTree
    //
    //
    //__________________________________________________________________________
    class SuffixTree
    {
    public:
        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
        explicit SuffixTree();          //!< setup with root
        virtual ~SuffixTree() noexcept; //!< cleanup

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________

        //! insertion
        /**
         \param path [0..size-1]
         \param size length of path, 0 means root
         \param data address to set in tree
         \return address of node if inserted, null otherwise
         */
        void  *      insert(const void *path, const size_t size, void *data);

        //! insertion wrapper
        void  *      insert(const Memory::ReadOnlyBuffer &, void *);

        //! reconstruct path as a String
        String       pathOf(const void *nodeAddr) const;

        //! search for data within tree
        const void  *search(const void *path, const size_t size) const noexcept;

        //! search wrapper
        const void  *search(const Memory::ReadOnlyBuffer & path) const noexcept;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(SuffixTree);
        class Code;
        Code *code;
    };

}

#endif

