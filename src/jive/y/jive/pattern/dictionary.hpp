
//! \file

#ifndef Y_Jive_Pattern_Dictionary_Included
#define Y_Jive_Pattern_Dictionary_Included 1

#include "y/string.hpp"

namespace Yttrium
{
    namespace Jive
    {
        class Pattern;

        //______________________________________________________________________
        //
        //
        //
        //! Dictionary of named patterns
        //
        //
        //______________________________________________________________________
        class Dictionary
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Dictionary();           //!< setup
            virtual ~Dictionary() noexcept;  //!< cleanup
            Y_OSTREAM_PROTO(Dictionary);     //!< display names

            void operator()(const char *   name, const Pattern *p); //!< append a newly created pattern
            void operator()(const String & name, const Pattern *p); //!< append a newly created pattern

            Pattern * create(const String &name);
            Pattern * create(const char   *name);


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Dictionary);
            class Code;
            Code *code;
        };

    }

}

#endif
