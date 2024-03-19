#include "y/apex/n/proto.hpp"
#include "y/text/plural.hpp"

namespace Yttrium
{
    namespace Apex
    {
        Y_SHALLOW_IMPL(AsShift);
        Y_SHALLOW_IMPL(AsByte);

        namespace Nexus
        {
            Proto:: ~Proto() noexcept {}
            Proto:: Proto() noexcept {}

            const double Proto::ScaleTable[64] =
            {
                1.0/double(Y_U64(0x0000000000000001)),
                1.0/double(Y_U64(0x0000000000000002)),
                1.0/double(Y_U64(0x0000000000000004)),
                1.0/double(Y_U64(0x0000000000000008)),
                1.0/double(Y_U64(0x0000000000000010)),
                1.0/double(Y_U64(0x0000000000000020)),
                1.0/double(Y_U64(0x0000000000000040)),
                1.0/double(Y_U64(0x0000000000000080)),
                1.0/double(Y_U64(0x0000000000000100)),
                1.0/double(Y_U64(0x0000000000000200)),
                1.0/double(Y_U64(0x0000000000000400)),
                1.0/double(Y_U64(0x0000000000000800)),
                1.0/double(Y_U64(0x0000000000001000)),
                1.0/double(Y_U64(0x0000000000002000)),
                1.0/double(Y_U64(0x0000000000004000)),
                1.0/double(Y_U64(0x0000000000008000)),
                1.0/double(Y_U64(0x0000000000010000)),
                1.0/double(Y_U64(0x0000000000020000)),
                1.0/double(Y_U64(0x0000000000040000)),
                1.0/double(Y_U64(0x0000000000080000)),
                1.0/double(Y_U64(0x0000000000100000)),
                1.0/double(Y_U64(0x0000000000200000)),
                1.0/double(Y_U64(0x0000000000400000)),
                1.0/double(Y_U64(0x0000000000800000)),
                1.0/double(Y_U64(0x0000000001000000)),
                1.0/double(Y_U64(0x0000000002000000)),
                1.0/double(Y_U64(0x0000000004000000)),
                1.0/double(Y_U64(0x0000000008000000)),
                1.0/double(Y_U64(0x0000000010000000)),
                1.0/double(Y_U64(0x0000000020000000)),
                1.0/double(Y_U64(0x0000000040000000)),
                1.0/double(Y_U64(0x0000000080000000)),
                1.0/double(Y_U64(0x0000000100000000)),
                1.0/double(Y_U64(0x0000000200000000)),
                1.0/double(Y_U64(0x0000000400000000)),
                1.0/double(Y_U64(0x0000000800000000)),
                1.0/double(Y_U64(0x0000001000000000)),
                1.0/double(Y_U64(0x0000002000000000)),
                1.0/double(Y_U64(0x0000004000000000)),
                1.0/double(Y_U64(0x0000008000000000)),
                1.0/double(Y_U64(0x0000010000000000)),
                1.0/double(Y_U64(0x0000020000000000)),
                1.0/double(Y_U64(0x0000040000000000)),
                1.0/double(Y_U64(0x0000080000000000)),
                1.0/double(Y_U64(0x0000100000000000)),
                1.0/double(Y_U64(0x0000200000000000)),
                1.0/double(Y_U64(0x0000400000000000)),
                1.0/double(Y_U64(0x0000800000000000)),
                1.0/double(Y_U64(0x0001000000000000)),
                1.0/double(Y_U64(0x0002000000000000)),
                1.0/double(Y_U64(0x0004000000000000)),
                1.0/double(Y_U64(0x0008000000000000)),
                1.0/double(Y_U64(0x0010000000000000)),
                1.0/double(Y_U64(0x0020000000000000)),
                1.0/double(Y_U64(0x0040000000000000)),
                1.0/double(Y_U64(0x0080000000000000)),
                1.0/double(Y_U64(0x0100000000000000)),
                1.0/double(Y_U64(0x0200000000000000)),
                1.0/double(Y_U64(0x0400000000000000)),
                1.0/double(Y_U64(0x0800000000000000)),
                1.0/double(Y_U64(0x1000000000000000)),
                1.0/double(Y_U64(0x2000000000000000)),
                1.0/double(Y_U64(0x4000000000000000)),
                1.0/double(Y_U64(0x8000000000000000))
            };

            size_t Proto:: BitsToBytes(const size_t numBits) noexcept
            {
                return Y_ALIGN_ON(8,numBits) >> 3;
            }

            const char Proto:: CallSign[] = "Apex::Natural";

            

            bool Proto:: CheckMetrics(const char  *where,
                                      const size_t nbits,
                                      const size_t bytes,
                                      const size_t words,
                                      const size_t WordSize,
                                      const size_t blockWords)
            {
                {
                    const size_t numBytes = BitsToBytes(nbits);
                    if( numBytes != bytes )
                    {
                        std::cerr << where << " bytes=" << bytes << " instead of " << numBytes << " for " << nbits << " bit" << Plural::s(nbits) << std::endl;
                        return false;
                    }
                }

                {
                    size_t count = bytes;
                    while(0!=(count%WordSize)) ++count;
                    count /= WordSize;
                    if(count!=words)
                    {
                        std::cerr << where << " words=" << words << " instead of " << count << " for " << bytes << " bytes" << Plural::s(bytes) << std::endl;
                        return false;
                    }
                }

                if(words>blockWords)
                {
                    std::cerr << where << " too many words=" << words << " instead or max " << blockWords << std::endl;
                    return false;
                }

                return true;
            }


        }

    }


}


