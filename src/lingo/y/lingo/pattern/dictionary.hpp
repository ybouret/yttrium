//! \file

#ifndef Y_Lingo_Pattern_Dictionary_Included
#define Y_Lingo_Pattern_Dictionary_Included 1

#include "y/lingo/pattern.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        

        //______________________________________________________________________
        //
        //
        //
        //! Dictionary of shared precompiled patterns
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
            explicit Dictionary();                            //!< setup empty
            virtual ~Dictionary() noexcept;                   //!< cleanup
            explicit Dictionary(const Dictionary &) noexcept; //!< shared copy
            Y_OSTREAM_PROTO(Dictionary);                      //!< display entries

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void define(const String &     name, Pattern * const p); //!< define new pattern
            void define(const char * const name, Pattern * const p); //!< define new pattern

            Pattern * compile(const String &     rx); //!< RegExp with this
            Pattern * compile(const char * const rx); //!< RegExp with this
            Pattern * compile(const char         rx); //!< RegExp with this

            //! set ID = RegExp(RX)
            template <typename ID, typename RX> inline
            void operator()(const ID &id, const RX &rx)
            {
                define(id,compile(rx));
            }



            Pattern * operator()(const String &)     const; //!< clone named pattern
            Pattern * operator()(const char * const) const; //!< clone named pattern


        private:
            Y_DISABLE_ASSIGN(Dictionary);
            class Code;
            Code *code;
        };
    }

}

#endif

