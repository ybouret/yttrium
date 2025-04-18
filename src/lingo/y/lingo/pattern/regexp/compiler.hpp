
//! file

#ifndef Y_Lingo_RegExp_Compiler_Included
#define Y_Lingo_RegExp_Compiler_Included 1

#include "y/lingo/pattern/dictionary.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        //______________________________________________________________________
        //
        //
        //
        //! Regular eXpression Compiler
        //
        //
        //______________________________________________________________________
        class RXC
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const CallSign; //!< "Regular Expression"
            static const char LBRACE = '{';     //!< alias
            static const char RBRACE = '}';     //!< alias
            static const char LPAREN = '(';     //!< alias
            static const char RPAREN = ')';     //!< alias
            static const char ALT    = '|';     //!< alias
            static const char LBRACK = '[';     //!< alias
            static const char RBRACK = ']';     //!< alias
            
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! initialize to persistent expression/dictionary
            explicit RXC(const char * const       _expr,
                         const size_t             _size,
                         const Dictionary * const _dict) noexcept;

            //! cleanup
            virtual ~RXC() noexcept;


            Pattern * subExpr(); //!< parse sub-expression starting at current
            Pattern * subBank(); //!< parse sub-bank starting at current

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const char * const       expr;  //!< original expression
            const char *             curr;  //!< current position
            const char * const       last;  //!< first invalid poistion
            const Dictionary * const dict;  //!< optional dictionary
            int                      depth; //!< current depth


        private:
            Y_DISABLE_COPY_AND_ASSIGN(RXC);
            static void Jokerize(Patterns &, const char); //!< single char jokers
            void        jkBraced(Patterns &);             //!< {alias} or counting/repeadted
            Pattern    *escapedSeq(const char * const);   //!< build escaped sequence
            Pattern    *subExprEsc();                     //!< escaped in subExpr
            Pattern    *subBankEsc();                     //!< escaped in subBank
            Pattern    *hexEsc();                         //!< hexadecimal escape
            Pattern    *getPosix();                       //!< [:named:]
            uint8_t     nextByte();                       //!< to make Lump in subBank
        };
    }
}

#endif

