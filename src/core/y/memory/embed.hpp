//! \file

#ifndef Y_Memory_Embed_Included
#define Y_Memory_Embed_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{
    namespace Memory
    {
        class Allocator;

        //______________________________________________________________________
        //
        //
        //
        //! embedding heterogeneous objects in flat memory
        //
        //
        //______________________________________________________________________
        class Embed
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! construct from base location and count of objects
            template <typename T>
            inline Embed( T * &entry, const size_t count) noexcept :
            handle( (void **)&entry   ),
            length( Align(count * sizeof(T)) ),
            offset( 0 )
            {
            }


            Embed(const Embed &) noexcept; //!< copy
            ~Embed()             noexcept; //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________


            //! ensure memory alignment
            static size_t Align(const size_t) noexcept;

            //! compute all metrics, allocate and link
            static void  *Build(Embed          embed[],
                                  const size_t count,
                                  Allocator   &alloc,
                                  size_t      &bytes);

        private:
            Y_DISABLE_ASSIGN(Embed);
            size_t nextOffset()     noexcept;
            void   link(void *base) noexcept;

            void **      handle;
            const size_t length;
            size_t       offset;
        };

    }

}


#endif

