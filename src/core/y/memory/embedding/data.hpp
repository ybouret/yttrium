
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
            //! data to store embedding info that cleans up upon destructor
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
                const size_t  size; //!< number of built Embed
                Embed * const data; //!< provided location of first Embed

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
                // Methods
                //
                //______________________________________________________________
                Embed       & operator[](const size_t indx)       noexcept; //!< access in [0..size()-1]
                const Embed & operator[](const size_t indx) const noexcept; //!< access in [0..size()-1], const
                
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                Y_OSTREAM_PROTO(Data);

                //! cleanup: all embeded are destructed, addresses are reset
                virtual ~Data()                noexcept;
            protected:
                explicit Data(void *ptr) noexcept; //!< setup with provided location

                //! push new information in place
                template <typename T> inline
                void push(T * &entry, const size_t count) noexcept
                {
                    assert(size<maxi());
                    new ( &data[Coerce(size)++] ) Embed(entry,count);
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Data);
            };

        }

    }

}

#endif

