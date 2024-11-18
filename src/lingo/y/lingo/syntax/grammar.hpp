//! \file

#ifndef Y_Lingo_Syntax_Grammar_Included
#define Y_Lingo_Syntax_Grammar_Included 1

#include "y/lingo/syntax/rules.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        class Parser;

        namespace Syntax
        {

            //__________________________________________________________________
            //
            //
            //
            //! Grammar is a list of rule
            //
            //
            //__________________________________________________________________
            class Grammar : public Entity, public Proxy<const Rules>
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef Aggregate     Agg;   //!< alias
                typedef Alternate     Alt;   //!< alias
                typedef Syntax::Rule  Rule;  //!< alias
                typedef Syntax::XNode XNode; //!< alias
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________

                //! setup empty with given name
                template <typename NAME> inline
                explicit Grammar(const NAME &   _name,
                                 Parser * const _host) :
                Entity(_name,AsCaption),
                rules(),
                parser(_host),
                locked(false)
                {
                }

                //! cleanup
                virtual ~Grammar() noexcept;



                //______________________________________________________________
                //
                //
                // Methods to construct grammar
                //
                //______________________________________________________________
                void         add(Rule * const  rule);              //!< add a new rule
                const Rule & top() const;                          //!< query top rule
                void         top(const Rule &);                    //!< set top rule
                void         validate();                           //!< not empty, all connected => locked
                
                //! helper to declare any derived Rule
                template <typename RULE> inline
                RULE & decl( RULE * const rule )
                {
                    assert(0!=rule);
                    add(rule);
                    return *rule;
                }

                //! get rule by name
                template <typename UUID> inline
                const Rule & operator[](const UUID &uuid) const
                {
                    const Rule * const rule = rules.query(uuid);
                    if(0==rule) {
                        const Caption ruleName(uuid);
                        throwMissing(ruleName);
                    }
                    return *rule;
                }


                const Rule &zom(const Rule & );              //!< zero or more rule
                const Rule &oom(const Rule & );              //!< one or more rule
                const Rule &rep(const Rule &, const size_t); //!< repeat rule at least some times
                const Rule &opt(const Rule &);               //!< rule as option

                //! helper to declare terminal
                template <typename NAME> inline
                const Rule & term__(const NAME &           _name,
                                    const Terminal::Kind   _kind,
                                    const Terminal::Role   _role)
                {
                    return decl( new Terminal(_name,_kind,_role) );
                }

                //! helper to declare a definite aggregate (permanent)
                template <typename NAME> inline
                Agg & agg(const NAME & _name) {
                    return decl( new Aggregate(_name,Aggregate::Definite,this) );
                }

                //! grouping only, won't appear in AST
                template <typename NAME> inline
                Agg & grp(const NAME & _name) {
                    return decl( new Aggregate(_name,Aggregate::Grouping,this) );
                }

                //! acting aggregate, no single:  merge content if only one member
                template <typename NAME> inline
                Agg & act(const NAME & _name) {
                    return decl( new Aggregate(_name,Aggregate::NoSingle,this) );
                }

                //! named altermation
                template <typename NAME> inline
                Alt & alt(const NAME & _name) {
                    return decl( new Alternate(_name,this) );
                }

                const Rule &pick(const Manifest &);                                           //!< alternation of manifest with at least two members
                const Rule &pick(const Rule &a, const Rule &b);                               //!< (a|b)
                const Rule &pick(const Rule &a, const Rule &b, const Rule &c);                //!< (a|b|c)
                const Rule &pick(const Rule &a, const Rule &b, const Rule &c, const Rule &d); //!< (a|b|c|d)

                const Rule &cat(const Manifest &);                                           //!< grouping of manifest with at least two members
                const Rule &cat(const Rule &a, const Rule &b);                               //!< (ab)
                const Rule &cat(const Rule &a, const Rule &b, const Rule &c);                //!< (abc)
                const Rule &cat(const Rule &a, const Rule &b, const Rule &c, const Rule &d); //!< (abcd)


                //! create/query a semantic/dividing terminal on the fly
                template <typename EXPR> inline
                const Rule & get(const EXPR &expr) {
                    if(0==parser) { const Caption request(expr); noParserFor(request);}
                    return get_(expr);
                }

                //! syntactic helper for grammar design
                template <typename EXPR> inline
                Grammar & operator<<(const EXPR &expr)
                {
                    (void) get(expr);
                    return *this;
                }


                //! render grammar
                void render() const;

                //______________________________________________________________
                //
                //
                // running grammar
                //
                //______________________________________________________________

                //! produce XNode from current lexer state and given source
                XNode * accept(Lexer &lexer, Source &source);


                //! reload a serialized node
                XNode * reload(InputStream &) const;

                //! helper to human readable exception
                void tryAppendTo(Exception           &excp,
                                 const char * const   prefix,
                                 const Lexeme * const lexeme) const;



            protected:
                void        no(const String &) noexcept;     //!< remove rule from unlocked grammar
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Grammar);
                Rules          rules;
                Parser * const parser;

                virtual ConstInterface & surrogate() const noexcept; //!< [Proxy]
                XNode * accepted(XNode * const, Lexer &, Source &);  //!< post-process tree
                void    rejected(const Lexer &);                     //!< guess what went wrong
                void    throwMissing(const Caption &) const;         //!< throw missing
                void    noParserFor(const Caption &)  const;

                const Rule & get_(const String &);        //!< to parser
                const Rule & get_(const char * const);    //!< to parser
                const Rule & get_(const char);            //!< to parser


            public:
                const bool  locked; //!< status
            };

        }

    }

}

#endif
