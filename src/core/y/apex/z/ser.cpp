#include "y/apex/integer.hpp"
#include "y/stream/output.hpp"
#include "y/stream/input.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{

    namespace Apex
    {
        
        size_t Integer:: serialize(OutputStream &fp) const
        {
            switch(s)
            {
                case __Zero__: fp.write(Is__Zero__); return 1;
                case Negative: fp.write(IsNegative); break;
                case Positive: fp.write(IsPositive); break;
            }
            assert(n>0);
            return 1 + n.serialize(fp);
        }

        static inline
        SignType mark2sign(const uint8_t mark)
        {
            switch( mark )
            {
                case Integer::Is__Zero__: return __Zero__;
                case Integer::IsNegative: return Negative;
                case Integer::IsPositive: return Positive;
            }
            throw Specific::Exception("Apex::Integer::Read","invalid mark sign 0x%02u",mark);
        }

        Integer Integer::Read(InputStream &fp)
        {
            const SignType theS  = mark2sign( fp.readCBR<uint8_t>("Apex::Integer Sign") );
            switch(theS)
            {
                case __Zero__: return 0;
                default:
                    break;
            }
            const Natural theN = Natural::Read(fp);
            return Integer(theS,theN);
        }

    }

}

