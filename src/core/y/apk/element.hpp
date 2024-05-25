//! \file

#ifndef Y_APK_Element_Included
#define Y_APK_Element_Included 1

#include "y/string.hpp"


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

        enum ElementState
        {
            AsBytes=1,
            AsNum16=2,
            AsNum32=4,
            AsNum64=8
        };

        class Element : public Object
        {
        public:



            // C++
            explicit Element(const size_t usrBytes=0);
            explicit Element(const size_t nbits, Random::Bits &ran);
            explicit Element(const Element &);
            virtual ~Element() noexcept;

            // Methods
            void ldz() noexcept;
            void ld(const uint64_t qw) noexcept;
            void set(const ElementState) noexcept;

            std::ostream & show(std::ostream &os) const; //!< show with current state

            static void CheckTransmogrify();

            // Members
            const size_t        bits;
            const ElementState state;
            const size_t       bytes;
            const size_t       num16;
            const size_t       num32;
            const size_t       num64;
            const unsigned     shift;
            const size_t       maxBytes;
        private:
            void * const       entry;
        public:
            const size_t       maxNum16;
            const size_t       maxNum32;
            const size_t       maxNum64;


        private:
            Y_DISABLE_ASSIGN(Element);
            static size_t MaxBytesFor(const size_t usrBytes, unsigned &shift);
            void convertToBytes() noexcept;
            void convertToNum16() noexcept;
            void convertToNum32() noexcept;
            void convertToNum64() noexcept;
        };


    }

}

#endif

