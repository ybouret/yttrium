#include "y/lingo/pattern/all.hpp"
#include "y/system/exception.hpp"


namespace Yttrium
{
    namespace Lingo
    {

        static inline Pattern * ReadLogic( AutoPtr<Logic> &p, InputStream &fp)
        {
            const size_t sz = fp.readVBR<size_t>("Logic.size");
            for(size_t i=0;i<sz;++i)
            {
                p->pushTail( Pattern::Read(fp) );
            }
            return p.yield();
        }

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
                case Counting::UUID: {
                    const size_t nmin = fp.readVBR<size_t>("Counting.minimalCount");
                    const size_t nmax = fp.readVBR<size_t>("Counting.maximalCount");
                    return Counting:: Create( Read(fp), nmin, nmax );
                }
                    // Logic

                case And::UUID: { AutoPtr<Logic> p = new And(); return ReadLogic(p,fp); }


                default:
                    throw Specific::Exception("Pattern::Read", "unknow UUID='%s'", FourCC::ToText(uid));
            }

            return 0;
        }

    }

}

