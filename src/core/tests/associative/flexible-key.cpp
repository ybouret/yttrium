#include "y/memory/buffer/ro.hpp"
#include "y/container/cxx/array.hpp"
#include "y/type/nullify.hpp"
#include "y/quantized.hpp"

#include "y/utest/run.hpp"

using namespace Yttrium;

namespace Yttrium
{
    template <typename T>
    class FlexibleKey : public Memory::ReadOnlyBuffer
    {
    public:

        Y_ARGS_DECL(T,Type);
        typedef CxxArray<MutableType> ArrayType;

        class Code : public Quantized, public ArrayType
        {
        public:
            inline explicit Code(ConstType * const entry,
                                 const size_t      words) :
            Quantized(),
            ArrayType(words,0),
            blockAddr(words>0? & (*this)[1] : 0),
            blockSize(words*sizeof(Type))
            {
                assert(Good(entry,words));
                for(size_t i=1,j=0;i<=words;++i,++j)
                {
                    (*this)[i] = entry[j];
                }
            }

            inline virtual ~Code() noexcept {}

            const void * const blockAddr;
            const size_t       blockSize;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);
        };

        inline FlexibleKey(ConstType * const entry,
                           const size_t      words) :
        code( new Code(entry,words) )
        {
        }

        inline virtual ~FlexibleKey() noexcept {  assert(0!=code); Nullify(code); }

        inline virtual const void * ro_addr() const noexcept {
            assert(0!=code);
            return code->blockAddr;
        }

        inline virtual size_t measure() const noexcept
        {
            assert(0!=code);
            return code->blockSize;
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(FlexibleKey);
        Code *             code;

    };
}

Y_UTEST(associative_flexible_key)
{

    Y_SIZEOF( FlexibleKey<uint8_t> );
    Y_SIZEOF( FlexibleKey<uint64_t> );
    Y_SIZEOF( FlexibleKey<uint8_t>::Code );
    Y_SIZEOF( FlexibleKey<uint64_t>::Code );

    {
        const uint8_t data[] = { 1,2,3,4 };
        FlexibleKey<uint8_t> key(data,sizeof(data)/sizeof(data[1]));

        key.displayHexadecimal(std::cerr) << std::endl;
    }

    {
        const uint32_t data[] = { 0x12345678,2,3,4 };
        FlexibleKey<uint32_t> key(data,sizeof(data)/sizeof(data[1]));

        key.displayHexadecimal(std::cerr) << std::endl;
    }

}
Y_UDONE()

