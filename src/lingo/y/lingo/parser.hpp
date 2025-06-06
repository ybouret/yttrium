//! \file

#ifndef Y_Lingo_Parser_Included
#define Y_Lingo_Parser_Included 1

#include "y/lingo/lexer.hpp"
#include "y/lingo/syntax/grammar.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        //______________________________________________________________________
        //
        //
        //
        //! Parser is a Grammar with a Lexer
        //
        //
        //______________________________________________________________________
        class Parser : public Syntax::Grammar
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef Syntax::Terminal    Terminal;                      //!< alias
            typedef Lexeme::Feat        Feat;                          //!< alias
            static const Feat           Drop     = Lexeme::Drop;       //!< alias
            static const Feat           Emit     = Lexeme::Emit;       //!< alias
            static const Terminal::Role Semantic = Terminal::Semantic; //!< alias
            static const Terminal::Role Dividing = Terminal::Dividing; //!< alias
            static const Terminal::Kind Univocal = Terminal::Univocal; //!< alias
            static const Terminal::Kind Standard = Terminal::Standard; //!< alias


            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup with name
            template <typename NAME> inline
            explicit Parser(const NAME & _name) :
            Syntax::Grammar(_name,this),
            lexer(name)
            {
            }

            virtual ~Parser() noexcept;

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! create a terminal with a given role
            template <typename UUID,typename EXPR,Terminal::Role ROLE> inline
            const Rule & term_(const UUID & uuid,
                               const EXPR & expr)
            {
                const Lexical::Rule &rule = lexer.emit(uuid,expr);
                const Terminal::Kind kind = rule.motif->univocal() ? Univocal : Standard;
                const Caption       &r_id = rule.name;
                try { return term__(r_id,kind,ROLE); }
                catch(...) { lexer.cut(rule); throw; }
            }

            //! create a semantic terminal
            template <typename UUID,typename EXPR> inline
            const Rule & term(const UUID & uuid,
                              const EXPR & expr)
            {
                return term_<UUID,EXPR,Semantic>(uuid,expr);
            }

            //! create a semantic terminal
            template <typename EXPR> inline
            const Rule & term(const EXPR &expr) {
                return term(expr,expr);
            }

            //! create a dividing terminal
            template <typename UUID,typename EXPR> inline
            const Rule & mark(const UUID & uuid,
                              const EXPR & expr)
            {
                return term_<UUID,EXPR,Dividing>(uuid,expr);
            }

            //! create a dividing terminal
            template <typename EXPR> inline
            const Rule & mark(const EXPR &expr) {
                return mark(expr,expr);
            }

            //! make a terminal 'uuid' from an AddOn
            template <typename ADD_ON> inline
            const Terminal & plug(const String &uuid) {
                const Terminal & xrule   = term__(uuid,Standard,Semantic);
                try { (void) lexer.plug<ADD_ON>(uuid); }
                catch(...) { no(uuid); throw; }
                return xrule;
            }

            //! make a terminal 'uuid' from an AddOn
            template <typename ADD_ON> inline
            const Terminal & plug(const char * const uuid) {
                const String _(uuid); return plug<ADD_ON>(_);
            }

            //! create an emitted end-line terminal
            template <typename UUID,typename EXPR> inline
            const Terminal & endl(const UUID & uuid, const EXPR &expr, const Terminal::Role role)
            {
                const Lexical::Rule &rule = lexer.endl(uuid,expr,Emit);
                const Terminal::Kind kind = rule.motif->univocal() ? Terminal::Univocal : Terminal::Standard;
                const Caption       &r_id = rule.name;
                try { return term__(r_id,kind,role); }
                catch(...) { lexer.cut(rule); throw; }
            }

            //! create an emittend end-line with name=expr
            template <typename EXPR> inline
            const Terminal & endl(const EXPR &expr, const Terminal::Role role)
            {
                return endl(expr,expr,role);
            }


          
            const Rule & get(const String &     expr); //!< on-the-fly create/query semantic
            const Rule & get(const char * const expr); //!< on-the-fly create/query semantic
            const Rule & get(const char         expr); //!< on-the-fly create/query dividing




            //! restart lexer and process module
            XNode * operator()(Module * const);

            //! restart lexer and process source
            XNode * operator()(Source &);


            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            Lexer lexer; //!< internal lexer, may be used to debug

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Parser);

        };

    }

}

#endif

