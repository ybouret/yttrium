//! \file

#ifndef Y_APK_Element_Included
#define Y_APK_Element_Included 1

#include "y/string.hpp"


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
            AsBytes=1, //!< uint8_t
            AsNum16=2, //!< uint16_t
            AsNum32=4, //!< uint32_t
            AsNum64=8  //!< uint64_t
        };

        //______________________________________________________________________
        //
        //
        //
        //! Versatile Element
        //
        //
        //______________________________________________________________________
        class Element : public Object
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const CallSign; //!< "APK::Element"
            static const InnerState   State[4]; //!< all the possible states

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Element(const size_t usrBytes=0);                //!< setup for usrBytes
            explicit Element(const size_t nbits, Random::Bits &ran);  //!< setup with exactly nbits
            explicit Element(const Element &);                        //!< full copy
            virtual ~Element() noexcept;                              //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________    
            Element  &     ldz()                   noexcept; //!< clean all, set AsBytes
            Element  &     u64(const uint64_t qw)  noexcept; //!< nothrow make u64
            Element  &     set(const InnerState)   noexcept;  //!< change inner state
            uint64_t       u64()             const noexcept; //!< check least significant uint64_t
            std::ostream & show(std::ostream &os)     const; //!< show with current state

            static InnerState   TuneUp(Element &lhs, Element &rhs) noexcept;

            //__________________________________________________________________
            //
            //
            // Comparisons
            //
            //__________________________________________________________________
            static SignType Compare(Element &lhs, Element &rhs) noexcept;
            static SignType Compare(Element &lhs, uint64_t rhs) noexcept;
            static SignType Compare(uint64_t lhs, Element &rhs) noexcept;


            static void CheckTransmogrify(); //!< internal tests

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________ 
            const size_t        bits;    //!< number of bits
            const InnerState   state;    //!< inner state
            const size_t       bytes;    //!< number of bytes    to hold bits
            const size_t       num16;    //!< number of uint16_t to hold bits
            const size_t       num32;    //!< number of uint32_t to hold bits
            const size_t       num64;    //!< number of uint64_t to hold bits
            const unsigned     shift;    //!< maxBytes = 2^shifht
            const size_t       maxBytes; //!< maximum number of bytes
        private:
            void * const       entry;    //!< internal memory
        public:
            const size_t       maxNum16; //!< maxBytes/2
            const size_t       maxNum32; //!< maxBytes/4
            const size_t       maxNum64; //!< maxBytes/8


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

