
//! \file

#ifndef Y_Lingo_Token_Included
#define Y_Lingo_Token_Included 1

#include "y/lingo/char.hpp"

namespace Yttrium
{
    namespace Apex { class Natural; }

    namespace Lingo
    {

        //______________________________________________________________________
        //
        //
        //
        //! List of Chars, for lexeme or internal caches
        //
        //
        //______________________________________________________________________
        class Token : public Char::List
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Token() noexcept; //!< setup
            virtual ~Token() noexcept; //!< cleanup
            Token(const Token &);      //!< copy
            Y_OSTREAM_PROTO(Token);    //!< display


            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            String        toString(const size_t skip=0,const size_t trim=0) const; //!< convert to string with optional clipping
            String        toPrintable() const;                                     //!< convert to printable string
            Apex::Natural toNatural()   const;                              //!< convert to integer
            Token & operator<<(Token &);                                    //!< merge back content
            Token & operator<<(Char * const ch) noexcept;                   //!< helper
            Token & operator+=(const Token &);                              //!< merge back a copy
            Token & operator+=(const Char  &);                              //!< push back a copy

            friend bool operator==(const Token &lhs, const Token &rhs) noexcept; //!< content equality
            friend bool operator==(const char * lhs, const Token &rhs) noexcept; //!< compare to string
            friend bool operator==(const char   lhs, const Token &rhs) noexcept; //!< compare to single char

            //! skip first bad
            template <typename PROC> inline
            Token & skip(PROC &isBad) noexcept
            {
                while(size>0 && isBad(**head)) delete popHead();
                return *this;
            }

            //! trim last bad
            template <typename PROC> inline
            Token & trim(PROC &isBad) noexcept
            {
                while(size>0 && isBad(**tail)) delete popTail();
                return *this;
            }

            //! trim and skip bad
            template <typename PROC> inline
            Token & crop(PROC &isBad) noexcept
            {
                return trim(isBad).skip(isBad);
            }


        private:
            Y_DISABLE_ASSIGN(Token);
        };
    }

}

#endif

