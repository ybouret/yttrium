
#include "y/lingo/lexer.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        Lexer:: ~Lexer() noexcept
        {
            analyzers.release();
            (void)liberate();
        }

        void Lexer:: initialize()
        {
            withhold();
            const Analyzer::Pointer ps = this;
            mustInsert(ps);
        }

        void Lexer:: mustInsert(const Analyzer::Pointer &ps)
        {
            if(!analyzers.insert(ps))
                throw Specific::Exception(name->c_str(),"unexpected multiple '%s'", ps->name->c_str());
        }



        void Lexer:: restart() noexcept
        {
            lexemes.release();
            history.free();
            analyzer = this;
        }

        void Lexer:: performCall(const Caption &label)
        {
            assert(0!=analyzer);
            Analyzer::Pointer *pps = analyzers.search( *label );
            if(0==pps)
                throw Specific::Exception(name->c_str(),"no [%s] to be called from [%s]", label->c_str(), analyzer->name->c_str());

            history << *analyzer;
            analyzer = & **pps;
        }

        void Lexer:: performBack()
        {
            assert(0!=analyzer);
            if(history.size<=0) throw Specific::Exception(name->c_str(),"cannot go back from [%s]", analyzer->name->c_str() );
            analyzer = & history.pullTail();
        }


        Lexeme * Lexer:: get(Source &source)
        {
        GET:
            assert(0!=analyzer);
            if(lexemes.size>0)
            {
                return lexemes.popHead();
            }
            else
            {
                Lexical::Result result = Lexical::Failure;
                AutoPtr<Lexeme> lexeme = analyzer->run(source,result);

                switch(result)
                {
                    case Lexical::Regular:
                        break; // will emit lexeme, NULL => EOF

                    case Lexical::Control:
                        if(lexeme.isValid()) throw Specific::Exception(analyzer->name->c_str(), "forbidden Control lexeme");
                        //throw Exception("Control lexeme not implemented");
                        goto GET;

                    case Lexical::Failure:
                        if(lexeme.isEmpty()) throw Specific::Exception(analyzer->name->c_str(), "failure without found reason");
                    {
                        const String        bad = lexeme->toPrintable();
                        Specific::Exception excp(analyzer->name->c_str(), "unexpected '%s'", bad.c_str());
                        lexeme->info.stamp(excp);
                        throw excp;
                    }

                }

                return lexeme.yield();
            }
        }
    }

}


