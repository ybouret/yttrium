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
                typedef Aggregate Agg;
                typedef Alternate Alt;
                typedef Option    Opt;
                typedef Repeat    Rep;

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
                // Methods
                //
                //______________________________________________________________
                void         add(Rule * const  rule); //!< add a new rule
                const Rule & top() const;             //!< query top rule
                void         top(const Rule &);       //!< set top rule
                void         validate();
                const Rule & operator[](const String &)     const;
                const Rule & operator[](const char * const) const;

                template <typename RULE> inline
                RULE & decl( RULE * const rule )
                {
                    assert(0!=rule);
                    add(rule);
                    return *rule;
                }


                const Rule &zom(const Rule & );
                const Rule &oom(const Rule & );
                const Rule &rep(const Rule &, const size_t);

                


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
