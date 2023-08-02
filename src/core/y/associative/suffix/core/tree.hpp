//! \file


#ifndef Y_Associative_Suffix_Core_Tree_Included
#define Y_Associative_Suffix_Core_Tree_Included 1

#include "y/string.hpp"

namespace Yttrium
{

    namespace Core
    {

        //______________________________________________________________________
        //
        //
        //
        //! Anonymous SuffixTree
        //
        //
        //______________________________________________________________________
        class SuffixTree
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit SuffixTree();          //!< setup with root
            virtual ~SuffixTree() noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

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
            Yttrium::String pathOf(const void *nodeAddr) const;

            //! check ownership
            bool owns(const void *nodeAddr) const noexcept;

            //! remove data
            void        loosen(void *nodeAddr) noexcept;

            //! search for data within tree
            const void *search(const void *path, const size_t size) const noexcept;

            void free()    noexcept; //!< empty structure
            void trim()    noexcept; //!< release unused memory
            void release() noexcept; //!< emptyy and keep only root

            //! search wrapper
            const void  *search(const Memory::ReadOnlyBuffer & path) const noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(SuffixTree);
            class Code;
            Code *code;
        };

    }
}

#endif

