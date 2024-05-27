//! \file

#ifndef Y_APK_Component_Included
#define Y_APK_Component_Included 1

#include "y/apk/component/assembly.hpp"
#include "y/object.hpp"
#include "y/random/bits.hpp"

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


        Y_SHALLOW_DECL(FromNum64);

        class Component : public Object
        {
        public:
            static const char * const CallSign;
            static const size_t       One =  1;
            static const InnerState   State[4];

            explicit Component(const size_t usrBytes);
            explicit Component(const size_t numBits, Random::Bits &ran);
            explicit Component(const Component &other);
            explicit Component(const FromNum64_ &, const uint64_t qw);
            virtual ~Component() noexcept;
            Y_OSTREAM_PROTO(Component);


            Component & set(const InnerState st) noexcept;
            uint64_t    u64() const noexcept; //!< get least significant u64

            static void * Tuned(const InnerState st, uint64_t &qw, size_t &nc) noexcept;
            static void   TuneUp(Component &lhs, Component &rhs) noexcept;




            const size_t             bits;
            const InnerState         state;
        private:
            unsigned                 shift;
            void *const              entry;
        public:
            const Assembly<uint8_t>  bytes;
            const Assembly<uint16_t> num16;
            const Assembly<uint32_t> num32;
            const Assembly<uint64_t> num64;


        private:
            Y_DISABLE_ASSIGN(Component);

            static void * EntryFor(const size_t usrBytes,
                                   unsigned    &usrShift);

        };

      

    }

}

#endif

