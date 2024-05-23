#include "y/utest/run.hpp"

#include "y/memory/allocator/archon.hpp"
#include "y/object.hpp"
#include "y/system/exception.hpp"
#include "y/type/utils.hpp"
#include "y/text/hexadecimal.hpp"
#include "y/calculus/base2.hpp"

#include <cstring>

using namespace Yttrium;


namespace Yttrium
{
    namespace APK
    {

        template <typename> struct MemOps;

        template <> struct MemOps<uint16_t>
        {
            static inline void WordToBytes(uint8_t * const byte, const uint16_t word) noexcept
            {
                byte[0] = static_cast<uint8_t>(word);
                byte[1] = static_cast<uint8_t>(word>>8);
            }

            static inline uint16_t BytesToWord(const uint8_t * const byte) noexcept
            {
                const uint16_t b0 = byte[0];
                const uint16_t b1 = byte[1];
                return b0 | (b1<<8);
            }

        };

        template <> struct MemOps<uint32_t>
        {
            static inline void WordToBytes(uint8_t * const byte, const uint32_t word) noexcept
            {
                byte[0] = static_cast<uint8_t>(word);
                byte[1] = static_cast<uint8_t>(word>>8);
                byte[2] = static_cast<uint8_t>(word>>16);
                byte[3] = static_cast<uint8_t>(word>>24);
            }

            static inline uint32_t BytesToWord(const uint8_t * const byte) noexcept
            {
                const uint32_t b0 = byte[0];
                const uint32_t b1 = byte[1];
                const uint32_t b2 = byte[2];
                const uint32_t b3 = byte[3];
                return b0 | (b1<<8) | (b2<<16) | (b3<<16);
            }
        };




        class Inner : public Object
        {
        public:
            static const size_t SZ1 = 1;
            enum State
            {
                AsBytes,
                AsWords
            };

            virtual ~Inner() noexcept {}

        protected:
            explicit Inner(const size_t usrBytes) :
            bits(0),
            bytes(0),
            shift(0),
            state(AsBytes),
            maxBytes( MaxBytesFor(usrBytes,Coerce(shift) ) ),
            byte( static_cast<uint8_t *>(Memory::Archon::Acquire( Coerce(shift) ) ) )
            {
                Coerce(maxBytes) = (SZ1 << shift);
            }

        public:

            inline void Make(const State target) noexcept
            {
                if(target!=state)
                {
                    switch(target)
                    {
                        case AsBytes: 
                            assert(AsWords==state);
                            WordsToBytes();
                            Coerce(state) = AsWords;
                            break;

                        case AsWords:
                            assert(AsBytes==state);
                            BytesToWords();
                            Coerce(state) = AsBytes;
                            break;
                    }
                }
            }



            const size_t    bits;
            const size_t    bytes;
            const unsigned  shift;
            const State     state;
            const size_t    maxBytes;
            uint8_t * const byte;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Inner);
            virtual void BytesToWords() const noexcept = 0;
            virtual void WordsToBytes() const noexcept = 0;

            static inline size_t MaxBytesFor(const size_t usrBytes, unsigned &shift)
            {
                if(usrBytes>=Base2<size_t>::MaxPowerOfTwo) throw Specific::Exception("APK","inner bytes overflow");

                size_t count = sizeof(uint64_t);
                shift        = iLog2<sizeof(uint64_t)>::Value; assert( (1<<shift) == count);
                while(count<usrBytes)
                {
                    count <<= 1;
                    ++shift;
                }
                return count;
            }
        };



        template <typename WORD>
        class Outer : public Inner
        {
        public:
            typedef WORD WordType;

            inline explicit Outer(const size_t usrBytes) :
            Inner(usrBytes),
            maxWords(maxBytes/sizeof(WORD)),
            words(0),
            word( (WordType *)byte )
            {
            }

            inline virtual ~Outer() noexcept {}

            const size_t     maxWords;
            const size_t     words;
            WordType * const word;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Outer);
            virtual void BytesToWords() const noexcept 
            {
                const uint8_t *b = byte;
                WORD          *w = word;
                for(size_t i=words;i>0;--i,b+=sizeof(WORD))
                    *(w++) = MemOps<WORD>::BytesToWord(b);
            }

            virtual void WordsToBytes() const noexcept
            {
                uint8_t    *b = byte;
                const WORD *w = word;
                for(size_t i=words;i>0;--i,b+=sizeof(WORD))
                    MemOps<WORD>::WordToBytes(b,*(w++));
            }
        };


        template <>
        class Outer<uint8_t> : public Inner
        {
        public:
            typedef uint8_t WordType;

            inline explicit Outer(const size_t usrBytes) noexcept :
            Inner(usrBytes),
            maxWords(maxBytes),
            words(bytes),
            word( (WordType *)byte )
            {
            }


            inline virtual ~Outer() noexcept {}

            const size_t &   maxWords;
            const size_t &   words;
            WordType * const word;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Outer);
            virtual void BytesToWords() const noexcept {}
            virtual void WordsToBytes() const noexcept {}
        };


    }
}

Y_UTEST(apk_n)
{

    for(size_t i=0;i<=20;++i)
    {
        std::cerr << i << std::endl;

        { APK::Outer<uint8_t>  el(i); }
        { APK::Outer<uint16_t> el(i); }
        { APK::Outer<uint32_t> el(i); }


    }

    Y_SIZEOF(APK::Outer<uint8_t>);
    Y_SIZEOF(APK::Outer<uint16_t>);
    Y_SIZEOF(APK::Outer<uint32_t>);


}
Y_UDONE()

