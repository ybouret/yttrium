
#include "y/lingo/syntax/terminal.hpp"
#include "y/utest/run.hpp"
#include "y/ptr/auto.hpp"

using namespace Yttrium;
using namespace Lingo;


Y_UTEST(syntax)
{

    Syntax::Terminal t1("INT",Syntax::Terminal::Standard,Syntax::Terminal::Semantic);
    GraphViz::Vizible::DotToPng("std-sem.dot",t1);

    Syntax::Terminal t2(":",Syntax::Terminal::Univocal,Syntax::Terminal::Semantic);
    GraphViz::Vizible::DotToPng("uni-sem.dot",t2);

    Syntax::Terminal t3("SEP",Syntax::Terminal::Standard,Syntax::Terminal::Dividing);
    GraphViz::Vizible::DotToPng("std-div.dot",t3);

    Syntax::Terminal t4(";",Syntax::Terminal::Univocal,Syntax::Terminal::Dividing);
    GraphViz::Vizible::DotToPng("uni-div.dot",t4);

}
Y_UDONE()
