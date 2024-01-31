
//! \file

#ifndef Y_Memory_Embedding_Data_Included
#define Y_Memory_Embedding_Data_Included 1

#include "y/memory/embed.hpp"
#include <new>

namespace Yttrium
{
    namespace Memory
    {
        namespace Embedding
        {
            //__________________________________________________________________
            //
            //
            //
            //! data to store embedding info
            //
            //
            //__________________________________________________________________
            class  Data
            {
            public:
                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                const size_t        size; //!< number of info
                const Embed * const data; //!< location

                //______________________________________________________________
                //
                //
                // Interface
                //
                //______________________________________________________________
                virtual size_t maxi() const noexcept = 0; //!< capacity

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                virtual ~Data()                noexcept; //!< cleanup
            protected:
                explicit Data(const void *ptr) noexcept; //!< setup

                //! push new information in place
                template <typename T> inline
                void push(T * &entry, const size_t count) noexcept
                {
                    assert(size<maxi());
                    const Embed &here = data[Coerce(size)++];
                    new ( (void *) &here ) Embed(entry,count);
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Data);
            };

        }

    }

}

#endif

