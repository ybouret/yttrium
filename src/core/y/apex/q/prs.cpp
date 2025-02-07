
#include "y/apex/rational.hpp"
#include "y/system/exception.hpp"
#include "y/container/algo/crop.hpp"

namespace Yttrium
{
    namespace Apex
    {
        Rational Rational:: Parse(const String &str)
        {
            String data = str;
            Algo::Crop(data,isspace);
            switch(data.size())
            {
                case 0: throw Specific::Exception(CallSign,"Parse Blank String");
                case 1: { const Natural N(data); return Rational(N); }
                default:
                    break;
            }
            if( '(' == data.head() )
            {
                if( ')' != data.tail() ) throw Specific::Exception(CallSign,"Missing Right Paren in String");
                char * const divSign = (char *)strchr(data.c_str(),'/');
                if(!divSign) throw Specific::Exception(CallSign,"Missing Div Sign in String");

                *divSign    = 0;
                data.tail() = 0;
                const String  ns = &data[2];
                const String  ds = divSign+1;
                const Integer nz = Integer::Parse(ns);
                const Natural dn = Natural(ds);
                return Rational(nz,dn);
            }
            else
            {
                const Integer Z(data);
                return Rational(Z);
            }

        }


        Rational::Rational(const String &s) :
        Number(), numer(0), denom(1)
        {
            Rational _ = Parse(s);
            (void) xch(_);
        }

        Rational & Rational:: operator=(const String &s)
        {
            Rational _ = Parse(s);
            return xch(_);
        }
    }

}

