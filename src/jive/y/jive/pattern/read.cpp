#include "y/jive/pattern/all.hpp"
#include "y/io/stream/input.hpp"
#include "y/system/exception.hpp"
#include "y/ptr/auto.hpp"
namespace Yttrium
{
    namespace Jive
    {

        static inline Pattern *ReadCompound(Compound *p, InputStream &fp)
        {
            assert(0!=p);
            AutoPtr<Compound> compound = p;
            p->retrieve(fp);
            return compound.yield();
        }

        Pattern * Pattern:: ReadFrom(InputStream &fp)
        {
            const uint32_t uuid = fp.readCBR<uint32_t>("Pattern.UUID");

            switch(uuid)
            {

                    // Basic
                case Any1::   UUID: return new Any1();
                case Single:: UUID: return new Single( fp.readCBR<uint8_t>("Single.code") );
                case Range::  UUID: {
                    const uint8_t lower = fp.readCBR<uint8_t>("Range.lower");
                    const uint8_t upper = fp.readCBR<uint8_t>("Range.upper");
                    return new Range(lower,upper);
                }
                case Exclude::UUID: return new Exclude( fp.readCBR<uint8_t>("Exclude.code") );

                    // Logic
                case And::  UUID:  return ReadCompound( new And(),  fp);
                case Or::   UUID:  return ReadCompound( new Or(),   fp);
                case None:: UUID:  return ReadCompound( new None(), fp);

                    // Joker
                case Optional::  UUID: return Optional::From( Pattern::ReadFrom(fp) );
                case Repeating:: UUID: {
                    const size_t nmin = fp.readVBR<size_t>("Repeating.atLeast");
                    return  Repeating::Make( nmin, Pattern::ReadFrom(fp) );
                }

                case Repeating:: ZOM: return ZeroOrMore( Pattern::ReadFrom(fp) );
                case Repeating:: OOM: return OneOrMore(  Pattern::ReadFrom(fp)  );


                default:
                    break;
            }
            throw Specific::Exception("Pattern::ReadFrom(InputStream)","%s %s",Core::Unknown,FourCC::ToText(uuid));
        }

    }

}
