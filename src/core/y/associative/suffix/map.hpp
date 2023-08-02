
//! \file


#ifndef Y_Associative_Suffix_Map_Included
#define Y_Associative_Suffix_Map_Included 1

#include "y/associative/suffix/alloy.hpp"
#include "y/associative/suffix/map/node.hpp"

namespace Yttrium
{

    namespace Core
    {
        class SuffixMap
        {
        public:
            static const char * const CallSign;

            explicit SuffixMap() noexcept;
            virtual ~SuffixMap() noexcept;


        private:
            Y_DISABLE_COPY_AND_ASSIGN(SuffixMap);
        };
    }

    template <typename KEY, typename T>
    class SuffixMap :
    public SuffixAlloy<KEY,T, SuffixMapNode<KEY,T>, Core::SuffixMap>
    {
    public:

        inline explicit SuffixMap() noexcept {}
        inline virtual ~SuffixMap() noexcept {}


    private:
        Y_DISABLE_COPY_AND_ASSIGN(SuffixMap);
    };

}

#endif

