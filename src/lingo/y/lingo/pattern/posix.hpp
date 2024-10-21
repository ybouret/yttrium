//! \file

#ifndef Y_Lingo_Pattern_Posix_Included
#define Y_Lingo_Pattern_Posix_Included 1

#include "y/lingo/pattern.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        //! compiling posix (and alike) patterns
        struct posix
        {

            //------------------------------------------------------------------
            //
            // standard
            //
            //------------------------------------------------------------------
            static Pattern *lower();  //!< \verbatim[a-z]\endverbatim
            static Pattern *upper();  //!< \verbatim[A-Z]\endverbatim
            static Pattern *alpha();  //!< \verbatim[a-zA-Z]\endverbatim
            static Pattern *digit();  //!< \verbatim[0-9]\endverbatim
            static Pattern *alnum();  //!< \verbatim[a-zA-Z0-9]\endverbatim
            static Pattern *xdigit(); //!< \verbatim[0-9a-fA-F]\endverbatim
            static Pattern *blank();  //!< \verbatim[ \t]\endverbatim
            static Pattern *space();  //!< \verbatim[ \n\r\t\v\f]\endverbatim
            static Pattern *punct();  //!< \verbatim][!"#$%&'()*+,./:;<=>?@\^_`{|}~-\endverbatim

            //------------------------------------------------------------------
            //
            // extended
            //
            //------------------------------------------------------------------
            static Pattern *word();      //!< \verbatim[a-zA-Z0-9_]\endverbatim
            static Pattern *endl();      //!< \verbatim\n|\r|\r\n\endverbatim
            static Pattern *dot();       //!< \verbatim^endl\endverbatim
            static Pattern *core();      //!< core 7-bits string content, minus quote, dquote  and backslash
            static Pattern *vowel();     //!< vowels
            static Pattern *consonant(); //!< consonants

        };
    }
}

#endif
