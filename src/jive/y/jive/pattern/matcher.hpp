//! \file

#ifndef Y_Jive_Pattern_Matcher_Included
#define Y_Jive_Pattern_Matcher_Included 1

#include "y/jive/pattern.hpp"
#include "y/jive/pattern/first-chars.hpp"
#include "y/jive/source.hpp"


namespace Yttrium
{
    namespace Jive
    {
        class Dictionary;

        //______________________________________________________________________
        //
        //
        //
        //! Matching regular expressions in data
        //
        //
        //______________________________________________________________________
        class Matcher
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            virtual ~Matcher() noexcept;                     //!< cleanup
            Matcher(Pattern *p) noexcept;                    //!< initialize from newly created pattern
            Matcher(const char   *, const Dictionary * = 0); //!< initialize from regexp
            Matcher(const String &, const Dictionary * = 0); //!< initialize from regexp
            Matcher(const Matcher &);                        //!< clone pattern, copy first chars

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void    cleanUp() noexcept;      //!< reset pattern
            Token * exactly(Source &source); //!< source must match exactly the pattern
            Token * somehow(Source &source); //!< source must contains the pattern

            //! use data as source to match exactly
            template <typename DATANAME, typename DATA>
            Token *exactly(DATANAME &dataName, DATA &data)
            {
                Source source( Module::Open(dataName,data) );
                return exactly(source);
            }

            //! use data as source to match somehow
            template <typename DATANAME, typename DATA>
            Token *somehow(DATANAME &dataName, DATA &data)
            {
                Source source( Module::Open(dataName,data) );
                return somehow(source);
            }

            

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const Motif      motif;        //!< compiled patter
            const FirstChars firstChars;   //!< pattern first chars

        private:
            Y_DISABLE_ASSIGN(Matcher);
            void initialize();
        };

    }
}

#endif

