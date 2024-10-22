
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
            static const char * const CallSign;
            static const char LBRACE = '{';
            static const char RBRACE = '}';
            static const char LPAREN = '(';
            static const char RPAREN = ')';
            static const char ALT    = '|';

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

            //! parse sub-expression starting at current
            Pattern * subExpr();

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
            static void Jokerize(Patterns &, const char);
            void        addAlias(Patterns &);
        };
    }
}

#endif

