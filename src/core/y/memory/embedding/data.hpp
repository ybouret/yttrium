
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
                const size_t  size; //!< number of info
                Embed * const data; //!< location

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

                //! cleanup: all embeded are destructed
                virtual ~Data()                noexcept;
            protected:
                explicit Data(void *ptr) noexcept; //!< setup
               
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

