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
                const Rule & operator[](const String &)     const; //!< access by name
                const Rule & operator[](const char * const) const; //!< access by nmae

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


                //! render grammar
                void render() const;

                //______________________________________________________________
                //
                //
                // running grammar
                //
                //______________________________________________________________
                XNode * accept(Lexer &lexer, Source &source);



            private:
                Y_DISABLE_COPY_AND_ASSIGN(Grammar);
                Rules rules;
                virtual ConstInterface & surrogate() const noexcept;
                
            public:
                const bool  locked; //!< status
            };

        }

    }

}

#endif
