//! \file

#ifndef Y_Lingo_Lexical_Scanner_Included
#define Y_Lingo_Lexical_Scanner_Included 1

#include "y/lingo/lexical/rule.hpp"
#include "y/lingo/pattern/dictionary.hpp"

#include "y/type/proxy.hpp"
#include "y/data/small/light/list/bare.hpp"
#include "y/memory/wad.hpp"
#include "y/memory/allocator/dyadic.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Lexical
        {

            //__________________________________________________________________
            //
            //
            // Definitions for Scanner
            //
            //__________________________________________________________________
            typedef Small::BareLightList<Rule>        RList; //!< list of reference to rules
            typedef RList::NodeType                   RNode; //!< alias
            typedef Memory::Wad<RList,Memory::Dyadic> RMaps; //!< memory for byte-indexed lists

            //! one run possible result
            enum Result
            {
                Regular, //!< found a Regular, emitted Unit
                Control, //!< found a Control unit for Lexer
                Failure  //!< syntax error
            };

            //__________________________________________________________________
            //
            //
            //
            //! Scanner to produce Units from a set of Rules
            //
            //
            //__________________________________________________________________
            class Scanner :
            public Quantized,
            public Entity,
            public RMaps,
            public Proxy<const Rules>
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                static const unsigned                     CHARS = 256; //!< alias




                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________

                //! setup from name and shared dictionary
                template <typename CAPTION>
                explicit Scanner(const CAPTION    &_name,
                                 const Dictionary &_dict) noexcept :
                Entity(_name,AsCaption),
                RMaps(CHARS),
                rlist(lead()),
                rules(),
                dict(_dict)
                {
                    initialize();
                }

                //! cleanup
                virtual ~Scanner() noexcept;

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

                //! compile any expression with internal dictionary
                template <typename RX> inline
                Pattern * compile(const RX &rx) {
                    return dict.compile(rx);
                }

                //! record a new rule
                void add(Rule * const rule);

                //! generic rule creation from regular expression
                template <
                typename ID,
                typename RX,
                typename HOST,
                typename METH> inline
                void on(const ID & id,
                        const RX & rx,
                        HOST     & host,
                        METH       meth)
                {
                    add( Rule::Create(id, compile(rx), host, meth) );
                }

                //! run over possible rules
                /**
                 - result = Regular:
                 -- Unit =  0 => EOF
                 -- Unit != 0 => Lexeme
                 - result = Control: Lexer is changed
                 - result = Failure: Unit is collected for Syntax Error
                 */
                Unit * run(Source &source,
                           Result &result);

                //! convert token to named unit
                Unit * produce(Token &) const;



                //! assign the proper built-in method for UUID='EXPR'
                template <
                typename   UUID,
                typename   EXPR,
                Unit::Feat FEAT,
                Unit::Spot SPOT
                > inline
                void fn(const UUID &uuid,
                        const EXPR &expr)
                {
                    on(uuid,expr,*this, & Scanner:: summon<FEAT,SPOT>);
                }

                //! bulk emit
                template <typename UUID, typename EXPR> inline
                void emit(const UUID &     uuid,
                          const EXPR &     expr)
                {
                    fn<UUID,EXPR,Unit::Emit,Unit::Bulk>(uuid,expr);
                }

                //! bulk drop
                template <typename UUID, typename EXPR> inline
                void drop(const UUID &     uuid,
                          const EXPR &     expr )
                {
                    fn<UUID,EXPR,Unit::Drop,Unit::Bulk>(uuid,expr);
                }

                //! bulk drop with uuid==expr
                template <typename EXPR> inline
                void drop(const EXPR &expr)
                {
                    drop(expr,expr);
                }


                //! endl with optional emit
                template <typename UUID, typename EXPR> inline
                void endl(const UUID & uuid,
                          const EXPR & expr,
                          Unit::Feat   feat)
                {
                    switch(feat)
                    {
                        case Unit::Drop: fn<UUID,EXPR,Unit::Drop,Unit::Endl>(uuid,expr); break;
                        case Unit::Emit: fn<UUID,EXPR,Unit::Emit,Unit::Endl>(uuid,expr); break;
                    }
                }

                //! endl with optional emit
                template <typename EXPR> inline
                void endl(const EXPR & expr,
                          Unit::Feat   feat)
                {
                    endl(expr,expr,feat);
                }



            private:
                Y_DISABLE_COPY_AND_ASSIGN(Scanner);
                RList * const rlist; //!< map of rules from their first chars
                Rules         rules; //!< list of rules

                void                     initialize() noexcept;       //!< setup map
                virtual ConstInterface & surrogate() const noexcept;  //!< [Proxy] rules

                Unit * findError(Source &source) const;

                template <Unit::Feat feat,Unit::Spot spot> inline
                Outcome summon(const Token &) const noexcept {
                    return Outcome(feat,spot);
                }

            public:
                //______________________________________________________________
                //
                //
                //  Members
                //
                //______________________________________________________________
                Dictionary  dict;  //!< shared dictionary
            };

        }

    }

}

#endif
