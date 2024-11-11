
#include "y/lingo/syntax/rules.hpp"
#include "y/utest/run.hpp"
#include "y/ptr/auto.hpp"
#include "y/stream/libc/output.hpp"
#include "y/lingo/syntax/rule/visit.hpp"

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

                virtual bool accepts(Y_Lingo_Syntax_Args) const
                {
                    (void)lexer;
                    (void)source;
                    (void)tree;
                    return false;
                }

                bool robust() const noexcept
                {
                    return true;
                }

                virtual const char * vizShape() const noexcept
                {
                    return "oval";
                }

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

    Syntax::Option opt(t1);
    {
        OutputFile fp("opt.dot");
        fp << "digraph G{\n";
        t1.viz(fp);
        opt.viz(fp);
        fp << "}\n";
    }
    GraphViz::Vizible::RenderPNG("opt.dot",false);

    Syntax::Repeat rep0(t1,0);
    {
        OutputFile fp("rep0.dot");
        fp << "digraph G{\n";
        t1.viz(fp);
        rep0.viz(fp);
        fp << "}\n";
    }
    GraphViz::Vizible::RenderPNG("rep0.dot",false);

    Syntax::Repeat rep1(t1,1);
    {
        OutputFile fp("rep1.dot");
        fp << "digraph G{\n";
        t1.viz(fp);
        rep1.viz(fp);
        fp << "}\n";
    }
    GraphViz::Vizible::RenderPNG("rep1.dot",false);

    Syntax::Repeat repN(t1,4);
    {
        OutputFile fp("repN.dot");
        fp << "digraph G{\n";
        t1.viz(fp);
        repN.viz(fp);
        fp << "}\n";
    }
    GraphViz::Vizible::RenderPNG("repN.dot",false);


    Syntax::Aggregate agg("agg");
    Syntax::Alternate alt("alt");


    Syntax::Visit visit;
    visit(t1);
    visit(opt);

}
Y_UDONE()
