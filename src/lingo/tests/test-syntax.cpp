
#include "y/lingo/syntax/terminal.hpp"
#include "y/lingo/syntax/internal.hpp"
#include "y/utest/run.hpp"
#include "y/ptr/auto.hpp"

using namespace Yttrium;
using namespace Lingo;

namespace Yttrium
{
    namespace Lingo
    {
        namespace Syntax
        {

            class Sim : public Internal
            {
            public:
                static const uint32_t UUID = Y_FOURCC('N', 'O', 'N', 'E');

                template <typename NAME>
                explicit Sim(const NAME & _name) : Internal(_name,UUID)
                {
                }

                virtual ~Sim() noexcept {}

                virtual void viz(OutputStream &) const {}

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Sim);
            };

        }

    }

}

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


    Syntax::Sim sim("SIM");

    AutoPtr<Syntax::XNode>  xnode = Syntax::XNode::CreateFrom(sim);


    Y_SIZEOF(Syntax::XNode);
    Y_SIZEOF(Syntax::XNode::List);
    Y_SIZEOF(Syntax::XList);

}
Y_UDONE()
