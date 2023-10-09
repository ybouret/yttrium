#include "y/jive/grammar.hpp"
#include "y/type/nullify.hpp"
#include "y/system/exception.hpp"
#include "y/associative/suffix/set.hpp"
#include "y/associative/address-book.hpp"
#include "y/text/plural.hpp"
#include "y/stream/xmlog.hpp"

namespace Yttrium
{
    namespace Jive
    {


        typedef SuffixSet<String,Syntax::Rule::Pointer> Rules;

        class Grammar:: Code : public Object, public Rules
        {
        public:

            inline explicit Code(const Tag &id) :
            Object(),
            Rules(),
            name(*id),
            entry(0),
            locked(false),
            xml( Syntax::Rule::Verbose )
            {
            }

            inline virtual ~Code() noexcept
            {

            }

            //__________________________________________________________________
            //
            //! insert a newly created rule
            //__________________________________________________________________
            inline void ins(Rule *rule)
            {
                assert(0!=rule);

                const Rule::Pointer ptr(rule);
                if(locked)
                    throw Specific::Exception(name.c_str(), "locked while inserting [%s]!", rule->name->c_str());

                if(!insert(ptr))
                    throw Specific::Exception(name.c_str(), "multiple rule [%s]",rule->name->c_str());

                if(!entry)
                    entry = rule;
            }

            //__________________________________________________________________
            //
            //! make an existing rule the top-level rule
            //__________________________________________________________________
            inline void top(const Rule &rule)
            {
                const Rule::Pointer *ppR = search( *(rule.name) );
                if(!ppR) throw Specific::Exception(name.c_str(), "no rule [%s]", rule.name->c_str() );
                const Rule &mine = **ppR;
                if( &mine != &rule) throw Specific::Exception(name.c_str(), "foreign rule [%s]", rule.name->c_str() );
                entry = & Coerce(mine);
            }

            //__________________________________________________________________
            //
            //! validation
            //__________________________________________________________________
            inline void validate()
            {
                //--------------------------------------------------------------
                // must not be locked
                //--------------------------------------------------------------
                if(locked)
                    throw Specific::Exception(name.c_str(), "locked while validating!");

                //--------------------------------------------------------------
                // must not be empty
                //--------------------------------------------------------------
                if(!entry)
                    throw Specific::Exception(name.c_str(), "empty grammar...");

                //--------------------------------------------------------------
                // check if all rules are accessible from top-level
                //--------------------------------------------------------------
                {
                    AddressBook book;
                    String      orphan;
                    unsigned    numBad = 0;
                    entry->endorse(book);
                    for(Iterator it=begin();it!=end();++it)
                    {
                        const Rule &rule = **it;
                        if(!book.search(rule))
                        {
                            ++numBad;
                            orphan += " [";
                            orphan += *rule.name;
                            orphan += "]";
                        }
                    }

                    if(numBad)
                        throw Specific::Exception(name.c_str(), "%u orphan%s:%s", numBad, Plural::s(numBad), orphan.c_str());
                }

                //--------------------------------------------------------------
                // check consistentcy of derived rules
                //--------------------------------------------------------------
                for(Iterator it=begin();it!=end();++it)
                {
                    const Rule &       rule = **it;
                    const char * const that = rule.name->c_str();
                    const uint32_t     uuid = rule.uuid;
                    switch(uuid)
                    {
                        case Syntax::Terminal::UUID:
                            break;

                        case Syntax::Aggregate::UUID:
                            if( rule.as<Syntax::Aggregate>()->size <= 0)
                                throw Specific::Exception( name.c_str(), "empty aggregate [%s]", that);
                            break;

                        case Syntax::Alternate::UUID:
                            if( rule.as<Syntax::Alternate>()->size <= 0)
                                throw Specific::Exception( name.c_str(), "empty alternate [%s]", that);
                            break;

                        case Syntax::Option::UUID:
                            if( rule.as<Syntax::Option>()->rule.isFrail())
                                throw Specific::Exception( name.c_str(), "frail option [%s]",that);
                            break;

                        case Syntax::Repeat::UUID:
                            if( rule.as<Syntax::Repeat>()->rule.isFrail())
                                throw Specific::Exception( name.c_str(), "frail repeat [%s]",that);
                            break;

                        default:
                            throw Specific::Exception( name.c_str(), "unknown rule [%s] (%s)", that, FourCC::ToText(uuid));
                    }
                }

                locked = true;

            }

            //__________________________________________________________________
            //
            //! output GraphViz
            //__________________________________________________________________
            void makeGraphViz(OutputStream &fp) const
            {
                fp << "digraph " << name << " {\n";

                // output 1/2: core code
                for(ConstIterator it=begin();it!=end();++it)
                {
                    const Rule &rule = **it;
                    rule.vizCore(fp);
                }

                // output 2/2: link code
                for(ConstIterator it=begin();it!=end();++it)
                {
                    const Rule &rule = **it;
                    rule.vizLink(fp);
                }

                fp << "}\n";
            }


            const String &name;     //!< reference to grammar name
            Rule         *entry;    //!< top-level rule
            bool          locked;   //!< status
            XMLog         xml;      //!< tracing

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);
        };

        Grammar::Code * Grammar::Initialize(const Tag &id)
        {
            return new Code(id);
        }
    }



}


namespace Yttrium
{
    namespace Jive
    {


        Grammar:: ~Grammar() noexcept
        {
            assert(0!=code);
            Nullify(code);
        }




        void Grammar:: ins(Rule *rule)
        {
            assert(0!=rule);
            assert(0!=code);
            code->ins(rule);
        }

        void Grammar:: topLevel(const Rule &rule)
        {
            assert(0!=code);
            code->top(rule);
        }

        void Grammar:: graphViz(OutputStream &fp) const
        {
            assert(0!=code);
            code->makeGraphViz(fp);
        }





        const Syntax::Rule & Grammar:: topLevel() const
        {
            assert(0!=code);
            if(0==code->entry) throw Specific::Exception( name->c_str(), "empty grammar, no top-level rule");
            return *(code->entry);
        }


        const Syntax::Rule & Grammar:: operator[](const String &ruleName) const
        {
            assert(0!=code);
            const Rule::Pointer *ppR = code->search( ruleName );
            if(!ppR) throw Specific::Exception( name->c_str(), "no rule [%s]", ruleName.c_str() );
            return **ppR;
        }

        const Syntax::Rule & Grammar:: zom(const Rule &rr)
        {
            return add( new Rep(rr,0) );
        }


        const Syntax::Rule & Grammar:: oom(const Rule &rr)
        {
            return add( new Rep(rr,1) );
        }

        const Syntax::Rule & Grammar:: opt(const Rule &rr)
        {
            return add( new Opt(rr) );
        }


        String * Grammar:: UID_(const Syntax::Manifest &m, const char sep)
        {
            AutoPtr<String> res = new String;
            {
                String         &s   = *res;
                s += '(';
                const Syntax::Manifest::NodeType *node = m.head;
                if(node)
                {
                    s += *((**node).name);
                    for(node=node->next;node;node=node->next)
                    {
                        s += sep;
                        s += *((**node).name);
                    }
                }
                s += ')';
            }
            return res.yield();
        }

        const Syntax::Rule & Grammar:: agg_(Syntax::Manifest &manifest)
        {
            Agg & res = add( new Agg( UID_(manifest,'#') ) );
            res.swapWith(manifest);
            return res;
        }

        const Syntax::Rule & Grammar:: cat(const Rule &a, const Rule &b)
        {
            Syntax::Manifest manifest;
            manifest << a << b;
            return agg_(manifest);
        }

        const Syntax::Rule & Grammar:: cat(const Rule &a, const Rule &b, const Rule &c)
        {
            Syntax::Manifest manifest;
            manifest << a << b << c;
            return agg_(manifest);
        }


        const Syntax::Rule & Grammar:: alt_(Syntax::Manifest &manifest)
        {
            Alt & res = add( new Alt( UID_(manifest,'|') ) );
            res.swapWith(manifest);
            return res;
        }

        const Syntax::Rule & Grammar:: pick(const Rule &a, const Rule &b)
        {
            Syntax::Manifest manifest;
            manifest << a << b;
            return alt_(manifest);
        }

        const Syntax::Rule & Grammar:: pick(const Rule &a, const Rule &b, const Rule &c)
        {
            Syntax::Manifest manifest;
            manifest << a << b << c;
            return alt_(manifest);
        }

        void Grammar:: validate()
        {
            assert(0!=code);
            code->validate();
        }



    }

}

#include "y/jive/source.hpp"

namespace Yttrium
{
    namespace Jive
    {

        Syntax::XNode * Grammar:: parse(Lexer &lexer, Source &source)
        {
            assert(0!=code);
            Y_XML_SECTION(code->xml,*name);
            if(0==code->entry) throw Specific::Exception( name->c_str(), "empty grammar");
            if(!code->locked)
            {
                Y_XMLOG(code->xml, "*** WARNING: not validated!!");
            }

            XTree tree = 0;
            if(!code->entry->accepts(lexer, source, tree, code->xml) )
            {
                rejected(lexer,source);
                assert( Say("never get here") );
            }

            return accepted(lexer,source,tree);

        }

        Syntax::XNode *Grammar:: accepted(Lexer &lexer, Source &source, XTree &tree)
        {
            assert(0!=code);
            Y_XML_SECTION(code->xml, "CheckAccepted");
            const Lexeme *lx = lexer.peek(source);

            if(lx)
            {
                cleanup(&Coerce(*lx));
                Specific::Exception excp(name->c_str(),"extraneous ");
                lx->stamp(excp);
                lx->publishIn(excp);
                Y_XMLOG(code->xml, "*** " << excp.when() );
                throw excp;
            }

            return XNode::AST( tree.yield() );
        }

        void Grammar:: rejected(Lexer &lexer, Source &source)
        {
            Y_XML_SECTION(code->xml, "CheckRejected");
            if(!lexer.peek(source))
            {
                Y_XMLOG(code->xml,"empty content");
                throw Specific::Exception(name->c_str(), "empty content is not supported");
            }

            const Lexeme *last  = lexer.last(); assert(0!=last);
            {
                cleanup(&Coerce(*last));
                Specific::Exception excp(name->c_str(),"invalid ");
                last->stamp(excp);
                last->publishIn(excp);
                const Lexeme *prev = last->prev;
                if(prev)
                {
                    cleanup(&Coerce(*prev));
                    excp.add(" after ");
                    prev->publishIn(excp);
                }
                Y_XMLOG(code->xml, "*** " << excp.when() );
                throw excp;
            }


            throw Specific::Exception(name->c_str(),"Unhandled error");
        }


        void Grammar:: cleanup(Lexeme *lexeme) const
        {
            assert(0!=code);
            assert(0!=lexeme);
            const String        &lid = lexeme->key();
            const Rule::Pointer *ppR = code->search(lid); if(!ppR)     throw Specific::Exception(name->c_str(), "no rule matching '%s'",  lid());
            const Rule          &rlx = **ppR; if(Term::UUID!=rlx.uuid) throw Specific::Exception(name->c_str(), "[%s] is not a Terminal", lid());
            if(rlx.as<Term>()->univocal)
                lexeme->release();
        }


    }

}

#include "y/vfs/vfs.hpp"

namespace Yttrium
{
    namespace Jive
    {
        void Grammar:: renderGraphViz(const bool keepDot) const
        {
            assert(0!=code);
            const String      dotFileName = VFS::ChangeExtension("dot", *name);
            Vizible::GraphViz(dotFileName,*this,keepDot);
        }
    }

}
