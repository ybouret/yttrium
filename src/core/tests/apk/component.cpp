#include "y/tow/api.hpp"
#include "y/memory/allocator/archon.hpp"
#include "y/utest/run.hpp"
#include "y/random/park-miller.hpp"
#include "y/calculus/bit-count.hpp"
#include "y/text/hexadecimal.hpp"
#include "y/type/utils.hpp"
#include "y/system/exception.hpp"
#include <cstring>

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


        template <typename T>
        class Assembly {
        public:
            inline Assembly(void * const blockAddr, 
                            const size_t numBlocks,
                            const size_t numActive=0) noexcept :
            count( numActive ),
            space( numBlocks ),
            entry( static_cast<T *>(blockAddr) )
            {
                assert(0!=entry);
                assert(space>0);
                assert(count<=space);
            }

            inline ~Assembly() noexcept {}

            inline friend std::ostream & operator<<(std::ostream &os, const Assembly &self)
            {
                os 
                << '[' << std::setw(4) << self.count << '/' << std::setw(4) << self.space << ']'
                << '@' << (const void *)(self.entry);
                Hexadecimal::Display(os << '=',self.entry,self.count);
                return os;
            }

            template <typename U> inline
            void load(const Assembly<U> &source)
            {
                // TARGET = T, SOURCE = U
                const size_t cycles = TOW::API<T,U>::Cycles(space,source.space);
                TOW::Transmute(entry,source.entry,cycles);
            }

            const size_t count; //!< valid entries
            const size_t space; //!< maximum count
            T * const    entry; //!< data

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Assembly);
        };




#define Y_APK_Component_Ctor()   \
bytes(entry,One  <<  shift),     \
num16(entry,bytes.space>>1),     \
num32(entry,num16.space>>1),     \
num64(entry,num32.space>>1)

        class Component : public Object
        {
        public:
            static const char * const CallSign;
            static const size_t       One =  1;

            explicit Component(const size_t usrBytes) :
            Object(),
            bits(0),
            state(AsBytes),
            shift(0),
            entry( EntryFor(usrBytes,shift) ),
            Y_APK_Component_Ctor()
            {
                std::cerr << "bytes : " << bytes << std::endl;
                std::cerr << "num16 : " << num16 << std::endl;
                std::cerr << "num32 : " << num32 << std::endl;
                std::cerr << "num64 : " << num64 << std::endl;

            }

            explicit Component(const size_t numBits, Random::Bits &ran) :
            Object(),
            bits(numBits),
            state(AsBytes),
            shift(0),
            entry(  EntryFor( Y_ALIGN8(bits)/8,shift) ),
            Y_APK_Component_Ctor()
            {
                if(bits>0)
                {
                    Coerce(bytes.count) = Y_ALIGN8(bits)/8; assert(bytes.count>0);
                    
                }
            }


            virtual ~Component() noexcept 
            {
                Memory::Archon::Release(entry,shift);
            }

            const size_t            bits;
            const InnerState        state;
        private:
            unsigned                shift;
            void *const             entry;
        public:
            const Assembly<uint8_t>  bytes;
            const Assembly<uint16_t> num16;
            const Assembly<uint32_t> num32;
            const Assembly<uint64_t> num64;


        private:
            Y_DISABLE_ASSIGN(Component);

            static void * EntryFor(const size_t usrBytes,
                                   unsigned    &usrShift)
            {
                static const unsigned MinShift64 = iLog2<sizeof(uint64_t)>::Value;
                static const unsigned MinShiftIO = Memory::Archon::MinShift;
                static const unsigned MinShift   = Max(MinShift64,MinShiftIO);
                static const size_t   MinBytes   = size_t(1) << MinShift;

                if(usrBytes>=Base2<size_t>::MaxPowerOfTwo) throw Libc::Exception(ENOMEM, "%s(%lu bytes)", CallSign, static_cast<unsigned long>(usrBytes));

                size_t count = MinBytes;
                usrShift     = MinShift;
                while(count<usrBytes)
                {
                    count <<= 1;
                    ++usrShift;
                }

                return Memory::Archon::Acquire(usrShift);
            }
        };

        const char * const Component:: CallSign = "APK::Component";


    }

}


using namespace Yttrium;

Y_UTEST(apk_component)
{

    APK::Component cm(0);

    Y_SIZEOF(APK::Component);
}
Y_UDONE()

