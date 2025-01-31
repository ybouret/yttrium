
#include "y/apex/rational.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Apex
    {
        Rational Rational:: Read(InputStream &fp)
        {
            const Integer D = Integer::Read(fp);
            const Natural N = Natural::Read(fp);
            if(N==0)                   throw Specific:: Exception(CallSign, "Read Zero Denominator");
            if(1!=Natural::GCD(D.n,N)) throw Specific:: Exception(CallSign,"Read Non Simplified");
            return Rational(D,N);
        }
    }
    
}
