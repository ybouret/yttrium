//! \file

#ifndef Y_APK_Element_Included
#define Y_APK_Element_Included 1

#include "y/object.hpp"


namespace Yttrium
{

#define Y_APK_Element_Ctor_Epilog(COUNT)         \
maxBytes( MaxBytesFor(COUNT,Coerce(shift) ) ),   \
entry( Memory::Archon::Acquire(Coerce(shift) ) ),\
maxNum16(maxBytes/sizeof(uint16_t)),             \
maxNum32(maxBytes/sizeof(uint32_t)),             \
maxNum64(maxBytes/sizeof(uint64_t))
    
    namespace APK
    {
        class Element : public Object
        {
        public:

            enum State
            {
                AsBytes,
                AsNum16,
                AsNum32,
                AsNum64
            };

            // C++
            explicit Element(const size_t usrBytes);
            explicit Element(const Element &);
            virtual ~Element() noexcept;

            // Methods
            void ldz() noexcept;
            void set(const uint64_t qw) noexcept;

            // Members
            const size_t   bits;
            const State    state;
            const size_t   bytes;
            const size_t   num16;
            const size_t   num32;
            const size_t   num64;
            const unsigned shift;
            const size_t   maxBytes;
        private:
            void * const   entry;
        public:
            const size_t   maxNum16;
            const size_t   maxNum32;
            const size_t   maxNum64;


        private:
            Y_DISABLE_ASSIGN(Element);
            static size_t MaxBytesFor(const size_t usrBytes, unsigned &shift);

        };


    }

}

#endif

