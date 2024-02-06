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
            length( AlignBytes(count * sizeof(T)) ),
            blocks( count )
            {
            }
            Embed(const Embed &) noexcept; //!< copy
            ~Embed()             noexcept; //!< cleanup handle (!)
            Y_OSTREAM_PROTO(Embed);        //!< dislplay

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________



            //! compute all metrics, allocate and link
            static void  *Build(Embed * const embed,
                                const size_t  count,
                                Allocator   & alloc,
                                size_t      & bytes);

            //! (check and) get allocated address
            void *   address() noexcept;

            //! get allocated/nil address
            const void * address() const noexcept;

            template <typename T> inline
            bool linkedTo(T * const &entry) const noexcept
            {
                return ( (void **) &entry ) == handle;
            }

        private:
            Y_DISABLE_ASSIGN(Embed);
            static size_t AlignBytes(const size_t) noexcept; //!< ensure memory alignment
            size_t        nextOffset()             noexcept; //!< offset+length
            void          linkOffset(void *base)   noexcept; //!< transfer
            void **      handle;
            size_t       offset;
        public:
            const size_t length;  //!< requested bytes
            const size_t blocks;  //!< allocated blocks (a.k.a objects)
        };

      


    }

}


#endif

