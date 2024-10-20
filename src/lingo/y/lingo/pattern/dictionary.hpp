//! \file

#ifndef Y_Lingo_Pattern_Dictionary_Included
#define Y_Lingo_Pattern_Dictionary_Included 1

#include "y/lingo/pattern.hpp"
#include "y/associative/suffix/map.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        //______________________________________________________________________
        //
        //
        // Types for Dictionary
        //
        //______________________________________________________________________
        typedef AutoPtr<const Pattern>             PatternPtr; //!< alias
        typedef SuffixMap<String,const PatternPtr> PatternMap; //!< alias

        //______________________________________________________________________
        //
        //
        //
        //! Dictionary of precompiled patterns
        //
        //
        //______________________________________________________________________
        class Dictionary : public PatternMap
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Dictionary();
            virtual ~Dictionary() noexcept;


            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void operator()(const String &     name, Pattern * const p); //!< insert new pattern
            void operator()(const char * const name, Pattern * const p); //!< insert new pattern

            Pattern * operator()(const String &);     //!< clone named pattern
            Pattern * operator()(const char * const); //!< cline named pattern


        private:
            Y_DISABLE_ASSIGN(Dictionary);
        };
    }

}

#endif

