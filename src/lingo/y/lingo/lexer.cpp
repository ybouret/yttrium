
#include "y/lingo/lexer.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        Lexer:: ~Lexer() noexcept
        {
            analyzers->release();
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
            if(!analyzers->insert(ps))
                throw Specific::Exception(name->c_str(),"inserting multiple analyzer '%s'", ps->name->c_str());
        }

        void  Lexer:: mustRecord(AddOn * const super)
        {
            assert(0!=super);
            const String &uuid = *(super->name);
            {
                const Analyzer::Pointer _(super);
                mustInsert(_);
            }
            try {
                const AddOn::Handle _(super);
                if(!addOns->insert(_))
                    throw Specific::Exception(name->c_str(),"inserting multiple AddOn<%s>", _->name->c_str());
            }
            catch(...)
            {
                (void) analyzers->remove(uuid);
                throw;
            }
        }


        void Lexer:: restart() noexcept
        {
            lexemes.release();
            history->free();
            analyzer = this;
        }

        void Lexer:: performCall(const Caption &label)
        {
            assert(0!=analyzer);
            Analyzer::Pointer *pps = analyzers->search( *label );
            if(0==pps)
                throw Specific::Exception(name->c_str(),"no [%s] to be called from [%s]", label->c_str(), analyzer->name->c_str());

            *history << *analyzer;
            analyzer = & **pps;
        }

        void Lexer:: performBack()
        {
            assert(0!=analyzer);
            if(history->size<=0) throw Specific::Exception(name->c_str(),"cannot go back from [%s]", analyzer->name->c_str() );
            analyzer = & history->pullTail();
        }

        void Lexer:: put(Lexeme * const lexeme) noexcept
        {
            assert(0!=lexeme);
            lexemes.pushHead(lexeme);
        }

        const char * Lexer:: here() const noexcept
        {
            assert(0!=analyzer);
            return analyzer->name->c_str();
        }

        


        Lexeme * Lexer:: get(Source &source, const Lexeme * const last)
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
                        if(lexeme.isValid()) throw Specific::Exception(here(), "forbidden Control lexeme");
                        goto GET;

                    case Lexical::Failure:
                        if(lexeme.isEmpty()) throw Specific::Exception(here(), "failure without found reason");
                        syntaxError(lexeme,last);
                }

                assert( Lexical::Regular == result );
                assert( 0!=analyzer );
                if(lexeme.isEmpty() )
                {
                    // we reached EOF
                    assert( !source.ready() );
                    const String &       uuid    = *(analyzer->name);
                    const AddOn::Handle *ppAddOn = addOns->search(uuid);
                    if(ppAddOn)
                    {
                        const AddOn &addOn = **ppAddOn;
                        switch(addOn.policy)
                        {
                            case Lexical::AddOn::RejectEndOfSource: {
                                const String descr = "in " + uuid + " of " + *(addOn.lexer.name);
                                Specific::Exception excp(descr.c_str(),"unexpected end of source");
                                source->stamp(excp);
                                throw excp;
                            }
                            case Lexical::AddOn::AcceptEndOfSource:
                                break;

                        }
                    }

                }

                return lexeme.yield();
            }
        }

        void Lexer:: syntaxError(const AutoPtr<Lexeme> &lexeme,
                                 const Lexeme * const   last) const
        {
            assert(lexeme.isValid());
            const String        bad = lexeme->toPrintable();
            Specific::Exception excp(here(), "syntax error '%s'", bad.c_str());
            lexeme->info.stamp(excp);
            if(last) {
                const String after = " after '" + last->toPrintable() + "'";
                excp.add("%s",after.c_str());
            }
            throw excp;
        }

        const Lexeme * Lexer:: peek(Source &source, const Lexeme * const last)
        {
           // std::cerr << "peek after last ";
            //if(last) std::cerr << *last << std::endl; else std::cerr << "(nil)" << std::endl;

            if(lexemes.size>0)
                return lexemes.head;
            else
            {
                Lexeme * const lexeme = get(source,last);
                if(lexeme) lexemes.pushTail(lexeme);
                return lexeme;
            }
        }

        
    }

}


