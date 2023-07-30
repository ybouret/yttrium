//! \file

#ifndef Y_Memory_Embed_Included
#define Y_Memory_Embed_Included 1

#include "y/config/starting.hpp"
#include "y/ostream-proto.hpp"

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
            offset( 0 ),
            length( Align(count * sizeof(T)) ),
            blocks( count )
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

            void * address() noexcept;

            Y_OSTREAM_PROTO(Embed);

        private:
            Y_DISABLE_ASSIGN(Embed);
            size_t nextOffset()     noexcept; //!< compute
            void   link(void *base) noexcept; //!< transfer
            void **      handle;
            size_t       offset;
        public:
            const size_t length;
            const size_t blocks;
        };

    }

}


#endif

