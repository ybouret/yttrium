//! \file

#ifndef Y_Lingo_Pattern_Dictionary_Included
#define Y_Lingo_Pattern_Dictionary_Included 1

#include "y/lingo/pattern.hpp"
#include "y/associative/suffix/map.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        typedef AutoPtr<const Pattern>             PatternPtr;
        typedef SuffixMap<String,const PatternPtr> PatternMap;

        class Dictionary : public PatternMap
        {
        public:
            explicit Dictionary();
            virtual ~Dictionary() noexcept;

            template <typename NAME> inline
            void operator()(const NAME &name, Pattern * const p) {
                const PatternPtr pp = p;
                if(!insert(name,pp)) { const String _(name); MultipleEntry(_); }
            }

        private:
            Y_DISABLE_ASSIGN(Dictionary);
            static void MultipleEntry(const String &);
        };
    }

}

#endif

