
#include "y/chemical/lang/rosary.hpp"
#include "y/chemical/reactive/actors.hpp"

#include "y/jive/parser.hpp"
#include "y/jive/syntax/translator.hpp"
#include "y/jive/lexical/plugin/rstring.hpp"

#include "y/type/temporary.hpp"
#include "y/system/exception.hpp"
#include "y/sequence/vector.hpp"
#include "y/apex/natural.hpp"
#include "y/container/algo/crop.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        const char * const Rosary::CallSign = "ChemicalRosary";


        namespace
        {
            class Players : public Object, public Actors
            {
            public:
                inline explicit Players() noexcept : Object(), Actors(), next(0), prev(0) {}
                inline virtual ~Players() noexcept {}

                Players *next;
                Players *prev;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Players);
            };


        }

        //______________________________________________________________________
        //
        //
        //
        // Compiler
        //
        //
        //______________________________________________________________________
        class Rosary::Compiler : public Jive::Parser, Jive::Syntax::Translator
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef Vector<String>     Strings;
            typedef Vector<int>        Charges;
            typedef Vector<unsigned>   Coefs;

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Compiler() :
            Jive::Parser(Rosary::CallSign),
            Jive::Syntax::Translator(),
            uuids(), 
            charges(),
            species(),
            coefs(),
            actors(),
            reac(),
            prod(),
            Ks(),
            hLib(0),
            hEqs(0)
            {
                setupParser();
                setupLinker();
            }

            inline virtual ~Compiler() noexcept
            {
            }

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! main processing
            void process(Jive::Module  *m,
                         Library       &lib,
                         LuaEquilibria &eqs)
            {
                //--------------------------------------------------------------
                // get AST
                //--------------------------------------------------------------
                const Temporary<Library *>       tlib(hLib,&lib); // set lib
                const Temporary<LuaEquilibria *> teqs(hEqs,&eqs); // set eqs
                Parser &                         self = *this;    // get parser
                AutoPtr<XNode>                   tree = self(m);  // get AST
                if(tree.isEmpty()) throw Specific::Exception(Rosary::CallSign, "Unexpected Empty Syntax Tree!");

                //--------------------------------------------------------------
                // post process: remove uneeded '+' between reac/prod
                //--------------------------------------------------------------
                GraphViz::Vizible::DotToPng( "rosary0.dot", *tree);
                apply(*tree, PostProcess, 0);
                GraphViz::Vizible::DotToPng( "rosary1.dot", *tree);

                //--------------------------------------------------------------
                // translate AST
                //--------------------------------------------------------------
                translate(*tree,Jive::Syntax::Permissive);
            }

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            Strings            uuids;
            Charges            charges;
            Species::SoloList  species;
            Coefs              coefs;
            Actors             actors;
            CxxListOf<Players> reac;
            CxxListOf<Players> prod;
            Strings            Ks;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Compiler);
            Library       *hLib;
            LuaEquilibria *hEqs;

            void setupParser();
            void setupLinker();

            void clearState() noexcept
            {
                uuids.  release();
                charges.release();
                species.release();
                coefs.  release();
                actors. release();
                reac.   release();
                prod.   release();
                Ks.     release();
            }

            virtual void initialize()
            {
                std::cerr << "*** Initializing " << name << std::endl;
                clearState();
            }

            static inline bool IsPLUS(const XNode &node) noexcept
            {
                return '+' == *(node.rule.name);
            }

            static inline void PostProcess(XNode &node, void *, const int) noexcept
            {
                if( "REAC" == *(node.rule.name) || "PROD" == (*node.rule.name))
                    node.removeChildIf(IsPLUS);
            }

            void onUUID(  const Jive::Token &token) { uuids << token.toString(); }
            void onPLUS(  const Jive::Token &)      {  }
            void onMINUS( const Jive::Token &)      {  }
            void onZPOS(size_t n) { assert(n>0); charges <<  int(n); }
            void onZNEG(size_t n) { assert(n>0); charges << -int(n); }


            //! create/query species with/without charge from library
            void onSPECIES(const size_t n)
            {
                //--------------------------------------------------------------
                // find name
                //--------------------------------------------------------------
                assert(1==n||2==n);
                assert(uuids.size()>0);
                String       name = uuids.pullTail();
                int          z    = 0;
                if(2==n)
                {
                    //----------------------------------------------------------
                    // find charge
                    //----------------------------------------------------------
                    assert(charges.size()>0);
                    z = charges.pullTail();
                }

                switch(Sign::Of(z))
                {
                    case __Zero__: break;
                    case Positive:
                        for(int i=0;i<z;++i) name += '+';
                        break;
                    case Negative:
                        for(int i=0;i< -z;++i) name += '-';
                        break;
                }

                //--------------------------------------------------------------
                // query lib
                //--------------------------------------------------------------
                Library       &lib  = *hLib;
                const Species &sp   = lib(name,z);

                //--------------------------------------------------------------
                // add to species
                //--------------------------------------------------------------
                species << sp;
            }

            inline void onCOEF(const Jive::Token &token)
            {
                apn nu = 0;
                for(const Jive::Char *ch=token.head;ch;ch=ch->next)
                {
                    const char code = **ch; assert(code>='0'); assert(code<='9');
                    nu *= 10;
                    nu += code - '0';
                }
                if(nu<=0) throw Specific::Exception(Rosary::CallSign, "zero stoichiometry");
                coefs << nu.cast<int>("nu");
            }

            inline void onACTOR(const size_t n)
            {
                assert( (1==n) || (2==n) );
                const Species &sp = species.pullTail();
                unsigned       nu = 1;
                if(2==n)
                    nu = coefs.pullTail();
                actors.pushTail( new Actor(nu,sp) );

            }

            inline void gather(Players * const target, size_t n) noexcept
            {
                assert(0!=target);
                while(n-- > 0 ) target->pushHead( actors.popTail() );
            }

            inline void onREAC(const size_t n)
            {
                gather( reac.pushTail( new Players() ), n);
                std::cerr << "reac: " << *reac.tail << std::endl;
            }

            inline void onPROD(const size_t n)
            {
                gather( prod.pushTail( new Players() ), n);
                std::cerr << "prod: " << *prod.tail << std::endl;
            }

            inline void onK(const Jive::Token &token)
            {
                String k = token.toString(1,1);
                Algo::Crop(k,isspace);
                std::cerr << "K=" << k << std::endl;
                Ks << k;
            }

            inline void onEQ(const size_t n)
            {
                assert(4==n);
                const String           name = uuids.pullTail();
                const AutoPtr<Players> r    = reac.popTail();
                const AutoPtr<Players> p    = prod.popTail();
                const String           k    = Ks.pullTail();

                std::cerr << name << ":" << r << "<=>" << p << ":'" << k << "'" << std::endl;

                if(k.size()<=0) throw Specific::Exception(Rosary::CallSign,"<%s> empty constant string", name.c_str());
                LuaEquilibria &eqs   = *hEqs;
                Lua::State    &L     = *eqs.lvm;
                const bool     isLua = isalpha(k[1]);
                Equilibrium   &eq    = (isLua ? eqs.makeLua(name,k) : eqs.make(name, L.eval<lua_Number>(k)) );

                for(const Actor *a = p->head;a;a=a->next) eq(int(a->nu),a->sp);
                for(const Actor *a = r->head;a;a=a->next) eq(-int(a->nu),a->sp);
            }

            inline void onROSARY(const size_t) noexcept
            { clearState(); }

        };

        // setting up parser
        void Rosary::Compiler::setupParser()
        {
            Agg       &ROSARY   = agg("ROSARY"); // top-level
            
            const Rule &PLUS    = term('+');
            const Rule &MINUS   = term('-');
            const Rule &UUID    = term("UUID","[:alpha:][:word:]*");
            const Rule &ZPOS    = (agg("ZPOS") << oom(PLUS));
            const Rule &ZNEG    = (agg("ZNEG") << oom(MINUS));
            const Rule &SPECIES = (agg("SPECIES") << mark('[') << UUID << opt( pick(ZPOS,ZNEG) ) << mark(']'));

            const Rule &COEF        = term("COEF","[:digit:]+");
            const Rule &OPT_COEF    = opt(COEF);
            const Rule &ACTOR       = (agg("ACTOR") << OPT_COEF << SPECIES);

            const Rule &FIRST_ACTOR = (grp("FIRST_ACTOR") << opt(PLUS) << ACTOR );
            const Rule &EXTRA_ACTOR = (grp("EXTRA_ACTOR") << PLUS << ACTOR);
            const Rule &ACTORS      = (grp("ACTORS") << FIRST_ACTOR << zom(EXTRA_ACTOR));
            const Rule &OPT_ACTORS  = opt(ACTORS);
            const Rule &REAC        = (agg("REAC") << OPT_ACTORS );
            const Rule &PROD        = (agg("PROD") << OPT_ACTORS );
            const Rule &CONSTANT    = plug<Jive::Lexical::RString>("K");
            const Rule &SEP         = mark(':');
            const Rule &EQ          = (agg("EQ") << UUID << SEP << REAC << mark("<=>")  << PROD << SEP << CONSTANT) ;

            ROSARY += zom(pick(SPECIES,EQ));


            renderGraphViz();
            lexer.drop("[:blank:]");
            lexer.endl("[:endl:]");
        }

        // setting up linker
        void Rosary::Compiler:: setupLinker()
        {
            Y_Jive_OnTerminal(Compiler,UUID);
            forTerminal('+', *this, &Compiler::onPLUS);
            forTerminal('-', *this, &Compiler::onMINUS);
            Y_Jive_OnInternal(Compiler,ZPOS);
            Y_Jive_OnInternal(Compiler,ZNEG);
            Y_Jive_OnInternal(Compiler,SPECIES);
            Y_Jive_OnTerminal(Compiler,COEF);
            Y_Jive_OnInternal(Compiler,ACTOR);
            Y_Jive_OnInternal(Compiler,REAC);
            Y_Jive_OnInternal(Compiler,PROD);
            Y_Jive_OnTerminal(Compiler,K);
            Y_Jive_OnInternal(Compiler,EQ);
            Y_Jive_OnInternal(Compiler,ROSARY);
        }


        static void *            RosaryCompiler_[ Y_WORDS_FOR(Rosary::Compiler) ];
        static Rosary::Compiler *RosaryCompiler = 0;


        Rosary:: Rosary()
        {
            std::cerr << "sizeof(Compiler)=" << sizeof(RosaryCompiler_) << std::endl;
            assert(0==RosaryCompiler);
            try {

                RosaryCompiler = new ( Y_STATIC_ZARR(RosaryCompiler_) ) Rosary::Compiler();
            }
            catch(...)
            {
                RosaryCompiler = 0;
                throw;
            }
        }

        Rosary:: ~Rosary() noexcept
        {
            assert(0!=RosaryCompiler);
            (void)Memory::OutOfReach::Naught(RosaryCompiler);
            RosaryCompiler = 0;
        }


        void Rosary:: operator()(Jive::Module  *m,
                                 Library       &lib,
                                 LuaEquilibria &eqs)
        {
            assert(0!=RosaryCompiler);
            RosaryCompiler->process(m,lib,eqs);
        }
    }

}

