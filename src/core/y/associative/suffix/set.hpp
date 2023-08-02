//! \file

#ifndef Y_Associative_Suffix_Set_Included
#define Y_Associative_Suffix_Set_Included 1

#include "y/associative/suffix/alloy.hpp"
#include "y/associative/suffix/set/node.hpp"

namespace Yttrium
{

    namespace Core
    {
        class SuffixSet
        {
        public:
            static const char * const CallSign;
            explicit SuffixSet() noexcept;
            virtual ~SuffixSet() noexcept;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(SuffixSet);
        };
    }

    template <typename KEY, typename T>
    class SuffixSet :
    public SuffixAlloy<KEY,T, SuffixSetNode<KEY,T>, Core::SuffixSet>
    {
    public:
        inline explicit SuffixSet() noexcept {}
        inline virtual ~SuffixSet() noexcept {}


    private:
        Y_DISABLE_COPY_AND_ASSIGN(SuffixSet);
    };

}

#endif

