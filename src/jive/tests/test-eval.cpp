#include "y/jive/parser.hpp"
#include "y/utest/run.hpp"
#include "y/ptr/auto.hpp"

//#include "y/jive/lexical/plugin/jstring.hpp"
#include "y/jive/lexical/plugin/single-line-comment.hpp"
#include "y/jive/lexical/plugin/multi-lines-comment.hpp"

using namespace Yttrium;

/*
 add  : mult ('+'^ mult)* ;        // left association
 mult : pow ('*'^ pow)* ;          // left association
 pow  : atom ('^'^ pow)? ;         // right association
 atom : ID | INT | '('^ add ')'! ; // recursion
 */
namespace
{
    class Eval : public Jive::Parser
    {
    public:
        explicit Eval() : Jive::Parser("Eval")
        {
        }

        virtual ~Eval() noexcept
        {
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Eval);
    };

}


Y_UTEST(eval)
{
    Eval eval;
    
}
Y_UDONE()

