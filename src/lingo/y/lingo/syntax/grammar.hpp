//! \file

#ifndef Y_Lingo_Syntax_Grammar_Included
#define Y_Lingo_Syntax_Grammar_Included 1

#include "y/lingo/syntax/rules.hpp"

namespace Yttrium
{
    namespace Lingo
    {
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
                typedef Aggregate Agg; //!< alias
                typedef Alternate Alt; //!< alias

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________

                //! setup empty with given name
                template <typename NAME> inline
                explicit Grammar(const NAME & _name) :
                Entity(_name,AsCaption),
                rules(),
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


                void        no(const String &) noexcept;     //!< remove rule from unlocked grammar
                const Rule &zom(const Rule & );              //!< zero or more rule
                const Rule &oom(const Rule & );              //!< one or more rule
                const Rule &rep(const Rule &, const size_t); //!< repeat rule at least some times
                const Rule &opt(const Rule &);               //!< rule as option

                //! helper to declare terminal
                template <typename NAME> inline
                const Rule & term_(const NAME &           _name,
                                   const Terminal::Kind   _kind,
                                   const Terminal::Role   _role)
                {
                    return decl( new Terminal(_name,_kind,_role) );
                }

                //! helper to declare a definite aggregate (permanent)
                template <typename NAME> inline
                Agg & agg(const NAME & _name) {
                    return decl( new Aggregate(_name,Aggregate::Definite) );
                }

                //! grouping only, won't appear in AST
                template <typename NAME> inline
                Agg & grp(const NAME & _name) {
                    return decl( new Aggregate(_name,Aggregate::Grouping) );
                }

                //! acting aggregate, no single:  merge content if only one member
                template <typename NAME> inline
                Agg & act(const NAME & _name) {
                    return decl( new Aggregate(_name,Aggregate::NoSingle) );
                }

                template <typename NAME> inline
                Alt & alt(const NAME & _name) {
                    return decl( new Alternate(_name) );
                }
                const Rule &pick(const Rule &a, const Rule &b);
                const Rule &pick(const Rule &a, const Rule &b, const Rule &c);
                const Rule &pick(const Rule &a, const Rule &b, const Rule &c, const Rule &d);
                const Rule &pick(const Manifest &);

                //! render grammar
                void render() const;

                //______________________________________________________________
                //
                //
                // running grammar
                //
                //______________________________________________________________
                XNode * accept(Lexer &lexer, Source &source);

                void tryAppendTo(Exception &excp, const Lexeme * const) const;


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Grammar);
                Rules rules;
                virtual ConstInterface & surrogate() const noexcept;

                XNode * accepted(XNode * const, Lexer &, Source &);
                void    rejected(Lexer &, Source &);

            public:
                const bool  locked; //!< status
            };

        }

    }

}

#endif
