
//! \file

#ifndef Y_Jive_Pattern_Dictionary_Included
#define Y_Jive_Pattern_Dictionary_Included 1

#include "y/string.hpp"

namespace Yttrium
{
    namespace Jive
    {
        class Pattern;

        
        class Dictionary
        {
        public:
            explicit Dictionary();
            virtual ~Dictionary() noexcept;
            Y_OSTREAM_PROTO(Dictionary);

            void operator()(const char *   name, const Pattern *p);
            void operator()(const String & name, const Pattern *p);


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Dictionary);
            class Code;
            Code *code;
        };

    }

}

#endif
