//! file


#ifndef Y_Hashing_Perfect_Included
#define Y_Hashing_Perfect_Included 1

#include "y/memory/buffer/ro.hpp"

namespace Yttrium
{
    namespace Hashing
    {
        //______________________________________________________________________
        //
        //
        //
        //! Perfect Hashing
        //
        //
        //______________________________________________________________________
        class Perfect
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Perfect();           //!< setup with root node
            virtual ~Perfect() noexcept;  //!< cleanup

            explicit Perfect(const char  *argv[],
                             const size_t argc);

            //__________________________________________________________________
            //
            //
            // Methods to record hash values
            //
            //__________________________________________________________________

            //! record data[size] with hash
            Perfect & operator()(const void * const data,
                                 size_t             size,
                                 const int          hash);
            
            Perfect & operator()(const Memory::ReadOnlyBuffer &buff, const int hash); //!< helper to record buffer
            Perfect & operator()(const char * const            text, const int hash); //!< helper to record text

            //__________________________________________________________________
            //
            //
            // Methods to retrieve hash values
            //
            //__________________________________________________________________
            int operator()(const void * const data, size_t size) const noexcept; //!< get hash value for data[size]
            int operator()(const Memory::ReadOnlyBuffer &  buff) const noexcept; //!< get hash value for buffer
            int operator()(const char * const              text) const noexcept; //!< get hash value for text

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Perfect);
            class Node;
            Node *root;
        };
    }
}

#endif
