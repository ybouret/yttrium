
//! \file

#ifndef Y_Jive_Pattern_Dictionary_Included
#define Y_Jive_Pattern_Dictionary_Included 1

#include "y/jive/pattern/regexp.hpp"

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

            void operator()(const char *   name, Pattern *p); //!< append a newly created pattern
            void operator()(const String & name, Pattern *p); //!< append a newly created pattern

            template <typename NAME, typename RX>
            inline void operator()(NAME &name, RX &rx)
            {
                Dictionary &dict = *this;
                Pattern *p = RegExp::Compile(rx,&dict);
                dict(name,p);
            }

            Pattern * create(const String &name) const; //!< return NULL on not found
            Pattern * create(const char   *name) const; //!< alias to create


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Dictionary);
            class Code;
            Code *code;
        };

    }

}

#endif
