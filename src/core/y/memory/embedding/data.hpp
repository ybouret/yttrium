
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

                //______________________________________________________________
                //
                //
                // Interface
                //
                //______________________________________________________________
                virtual size_t maxi() const noexcept = 0; //!< capacity

                Embed       & operator[](const size_t indx)       noexcept;
                const Embed & operator[](const size_t indx) const noexcept;

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                virtual ~Data()                noexcept; //!< cleanup
            protected:
                explicit Data(void *ptr) noexcept; //!< setup
                Embed * const data; //!< location

                //! push new information in place
                template <typename T> inline
                void push(T * &entry, const size_t count) noexcept
                {
                    assert(size<maxi());
                    Embed &here = data[Coerce(size)++];
                    new ( &here ) Embed(entry,count);
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Data);
            };

        }

    }

}

#endif

