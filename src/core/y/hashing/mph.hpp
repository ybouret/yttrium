//! file


#ifndef Y_Hashing_Minimal_Perfect_Included
#define Y_Hashing_Minimal_Perfect_Included 1

#include "y/memory/buffer/ro.hpp"

namespace Yttrium
{
    namespace Hashing
    {
        //______________________________________________________________________
        //
        //
        //
        //! Minimal Perfect Hashing
        //
        //
        //______________________________________________________________________
        class MinimalPerfect
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit MinimalPerfect();           //!< setup with root node
            virtual ~MinimalPerfect() noexcept;  //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods to record hash values
            //
            //__________________________________________________________________

            //! record data[size] with hash
            MinimalPerfect & operator()(const void * const data,
                                        size_t             size,
                                        const int          hash);

            MinimalPerfect & operator()(const Memory::ReadOnlyBuffer &buff, const int hash); //!< helper to record buffer
            MinimalPerfect & operator()(const char * const            text, const int hash); //!< helper to record text


            int operator()(const void * const data, size_t size) const noexcept; //!< get hash value for data[size]
            int operator()(const Memory::ReadOnlyBuffer &  buff) const noexcept; //!< get hash value for buffer
            int operator()(const char * const              text) const noexcept; //!< get hash value for text

        private:
            Y_DISABLE_COPY_AND_ASSIGN(MinimalPerfect);
            class Node;
            Node *root;
        };
    }
}

#endif

