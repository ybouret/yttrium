#include "y/lingo/pattern/all.hpp"
#include "y/system/exception.hpp"


namespace Yttrium
{
    namespace Lingo
    {

        Pattern * Pattern:: Read(InputStream &fp)
        {
            const uint32_t uid = fp.readCBR<uint32_t>("Pattern UUID");

            switch(uid)
            {
                    // Basic
                case Single::UUID:
                    return new Single( fp.readCBR<uint8_t>("Single.byte") );

                case Range::UUID: {
                    const uint8_t lower = fp.readCBR<uint8_t>("Range.lower");
                    const uint8_t upper = fp.readCBR<uint8_t>("Range.upper");
                    return new Range(lower,upper);
                }

                case Exclude::UUID:
                    return new Exclude( fp.readCBR<uint8_t>("Exclude.byte") );


                    // Joker
                case Optional::UUID: return Optional::Create( Read(fp) );
                case Repeated::UUID: {
                    const size_t nmin = fp.readVBR<size_t>("Repeated.minimalCount");
                    return Repeated:: Create( Read(fp), nmin );
                }


                default:
                    throw Specific::Exception("Pattern::Read", "unknow UUID='%s'", FourCC::ToText(uid));
            }

            return 0;
        }

    }

}

