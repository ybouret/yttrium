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


