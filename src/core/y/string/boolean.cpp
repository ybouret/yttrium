
#include "y/string/boolean.hpp"
#include "y/string.hpp"
#include "y/text/ops.hpp"
#include "y/system/exception.hpp"
#include <cstring>

namespace Yttrium
{
    static const char * MeaningTrue[] =
    {
        "1", "true", "on"
    };

    static const char * MeaningFalse[] =
    {
        "0", "false", "off"
    };


    static bool IsWithin(const char * argv[],
                         const size_t  argc,
                         const char * const query) noexcept
    {
        assert(0!=argv);
        assert(argc>0);

        for(size_t i=0;i<argc;++i)
        {
            if( 0 == strcmp(argv[i],query) ) return true;
        }
        return false;
    }

#define Y_IS_WITHIN(ARR,QUERY) IsWithin(ARR,sizeof(ARR)/sizeof(ARR[0]),QUERY)

    bool StringToBoolean:: MeansTrue(const char * const lower) noexcept
    {
        assert(0!=lower);
        return Y_IS_WITHIN(MeaningTrue,lower);
    }

    bool StringToBoolean:: MeansFalse(const char * const lower) noexcept
    {
        assert(0!=lower);
        return Y_IS_WITHIN(MeaningFalse,lower);
    }


    bool StringToBoolean:: Get(const char * const buffer, const size_t buflen, const char * field)  
    {
        assert( Good(buffer,buflen) );
        const String query(buffer,buflen);
        const char * const rhs = query.c_str();
        TextOps::ToLower( (char *)rhs,query.size());
        if( MeansTrue(rhs)  ) return true;
        if( MeansFalse(rhs) ) return false;
        throw Specific::Exception("StringToBoolean", "'%s' is not a booleang string", ( 0!=field ? field : Core::Unknown) );
    }

    bool StringToBoolean:: Get(const Core::String<char> &name, const char *field)
    {
        return Get(name.c_str(), name.size(), field);
    }



}
