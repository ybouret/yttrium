
//! \file

#ifndef Y_Concurrent_SIMD_Included
#define Y_Concurrent_SIMD_Included 1

#include "y/concurrent/loop/interface.hpp"
#include "y/ostream-proto.hpp"

namespace Yttrium
{
    namespace Concurrent
    {

        //______________________________________________________________________
        //
        //
        //! Range of 1D indices to perform operations
        //
        //______________________________________________________________________
        class Range
        {
        public:
            inline  Range() noexcept; //!< setup zero
            inline ~Range() noexcept; //!< cleanup
            Y_OSTREAM_PROTO(Range);

            const size_t length; //!< number of indices
            const size_t offset; //!< first index

            template <typename SEQUENCE> inline
            bool matches(const SEQUENCE &seq) const noexcept
            {
                return 1 == offset && seq.size() == length;
            }



        private:
            Y_DISABLE_ASSIGN(Range);
        };

        typedef Readable<const Range> Ranges;

        //______________________________________________________________________
        //
        //
        //
        //! SIMD for 1D operations
        //
        //
        //______________________________________________________________________
        class SIMD : public Ranges
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const CallSign; //!< "SIMD"

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit SIMD(const SharedLoop &); //!< setup from shared loop
            explicit SIMD(Loop             *); //!< setup from new loop
            virtual ~SIMD() noexcept;

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! setup ranges
            /**
             \param dataLength global data length
             \param dataOffset global data offset
             \return number of active ranges
             */
            size_t dispatch(const size_t dataLength, const size_t dataOffset=1) noexcept;

            //! test call
            void operator()(void) noexcept;

            

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________
            virtual size_t       size()                   const noexcept; //!< loop->size()
            virtual const char * callSign()               const noexcept; //!< CallSign
            virtual const Range &operator[](const size_t) const noexcept; //!< in [1..size()]

        private:
            Y_DISABLE_COPY_AND_ASSIGN(SIMD);
            class Code;
            SharedLoop  loop;
            Code       *code;

            struct Call0
            {
                const Ranges &ranges;
                void operator()(const ThreadContext &ctx) const;
            };


        public:
            const Range full; //!< last global matching range
        };

    }

}

#endif
