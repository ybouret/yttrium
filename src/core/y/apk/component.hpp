//! \file

#ifndef Y_APK_Component_Included
#define Y_APK_Component_Included 1

#include "y/apk/component/assembly.hpp"
#include "y/object.hpp"
#include "y/random/bits.hpp"
#include "y/type/signs.hpp"

namespace Yttrium
{
    namespace APK
    {
        //______________________________________________________________________
        //
        //
        //! Inner State
        //
        //______________________________________________________________________
        enum InnerState
        {
            AsBytes, //!< uint8_t
            AsNum16, //!< uint16_t
            AsNum32, //!< uint32_t
            AsNum64  //!< uint64_t
        };


        Y_SHALLOW_DECL(FromNum64); //!< alias


        //______________________________________________________________________
        //
        //
        //
        //! Versatile Little Endian Component
        //
        //
        //______________________________________________________________________
        class Component : public Object
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const CallSign; //!< "APK::Component"
            static const size_t       One =  1; //!< alias
            static const InnerState   State[4]; //!< aliases

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Component(const size_t usrBytes);                   //!< setup for bytes
            explicit Component(const size_t numBits, Random::Bits &ran); //!< setup for random bits
            explicit Component(const Component &other);                  //!< copy
            explicit Component(const FromNum64_ &, const uint64_t qw);   //!< setup for uint64_t
            virtual ~Component() noexcept;                               //!< cleanup
            Y_OSTREAM_PROTO(Component);                                  //!< display

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            Component & set(const InnerState st) noexcept; //!< change internal state
            uint64_t    u64() const noexcept;              //!< get least significant u64

            
            static InnerState TuneUp(Component &lhs,  Component &rhs) noexcept; //!< tune to the highest word
            static SignType   Compare(Component &lhs, Component &rhs) noexcept; //!< comparison


            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const size_t             bits;  //!< bits
            const InnerState         state; //!< inner state
        private:
            unsigned                 shift; //!< bytes.space = 2^shift
            void *const              entry; //!< memory entry
        public:
            const Assembly<uint8_t>  bytes; //!< as uint8_t
            const Assembly<uint16_t> num16; //!< as uint16_t
            const Assembly<uint32_t> num32; //!< as uint32_t
            const Assembly<uint64_t> num64; //!< as uint64_t


        private:
            Y_DISABLE_ASSIGN(Component);

            static void * EntryFor(const size_t usrBytes,
                                   unsigned    &usrShift);

        };

      

    }

}

#endif

