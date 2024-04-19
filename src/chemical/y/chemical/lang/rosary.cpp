
#include "y/chemical/lang/rosary.hpp"
#include "y/chemical/reactive/actors.hpp"

#include "y/chemical/lang/parser.hpp"
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
        class Rosary::Compiler : public Parser, public Jive::Syntax::Translator
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
            Parser(Rosary::CallSign),
            Jive::Syntax::Translator(),
            uuid(),
            algZ(),
            spec(),
            coef(),
            cast(),
            reac(),
            prod(),
            Kstr(),
            hLib(0),
            hEqs(0)
            {
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
                clearState();
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
            Strings            uuid;
            Charges            algZ;
            Species::SoloList  spec;
            Coefs              coef;
            Actors             cast;
            CxxListOf<Players> reac;
            CxxListOf<Players> prod;
            Strings            Kstr;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Compiler);
            Library       *hLib;
            LuaEquilibria *hEqs;

            void setupLinker();

            void clearState() noexcept
            {
                uuid.release();
                algZ.release();
                spec.release();
                coef.release();
                cast.release();
                reac.release();
                prod.release();
                Kstr.release();
            }



            static inline bool IsPLUS(const XNode &node) noexcept
            {
                return '+' == *(node.rule.name);
            }

            static inline void PostProcess(XNode &node, void *, const int) noexcept
            {
                const String &id = *(node.rule.name);
                if( "REAC" == id  || "PROD" == id)
                    node.removeChildIf(IsPLUS);
            }

            void onUUID(  const Jive::Token &token) { uuid << token.toString(); }
            void onPLUS(  const Jive::Token &)      {  }
            void onMINUS( const Jive::Token &)      {  }
            void onZPOS(size_t n) { assert(n>0); algZ <<  int(n); }
            void onZNEG(size_t n) { assert(n>0); algZ << -int(n); }


            //! create/query species with/without charge from library
            void onSPECIES(const size_t n)
            {
                //--------------------------------------------------------------
                // find name
                //--------------------------------------------------------------
                assert(1==n||2==n);
                assert(uuid.size()>0);
                String       name = uuid.pullTail();
                int          z    = 0;
                if(2==n)
                {
                    //----------------------------------------------------------
                    // find charge
                    //----------------------------------------------------------
                    assert(algZ.size()>0);
                    z = algZ.pullTail();
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
                spec << sp;
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
                coef << nu.cast<int>("nu");
            }

            inline void onACTOR(const size_t n)
            {
                assert( (1==n) || (2==n) );
                const Species &sp = spec.pullTail();
                unsigned       nu = 1;
                if(2==n)
                    nu = coef.pullTail();
                cast.pushTail( new Actor(nu,sp) );

            }

            inline void gather(Players * const target, size_t n) noexcept
            {
                assert(0!=target);
                while(n-- > 0 ) target->pushHead( cast.popTail() );
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
                Kstr << k;
            }

            inline void onEQ(const size_t n)
            {
                assert(4==n);
                const String           name = uuid.pullTail();
                const AutoPtr<Players> r    = reac.popTail();
                const AutoPtr<Players> p    = prod.popTail();
                const String           k    = Kstr.pullTail();

                std::cerr << name << ":" << r << "<=>" << p << ":'" << k << "'" << std::endl;

                if(k.size()<=0) throw Specific::Exception(Rosary::CallSign,"<%s> empty constant string", name.c_str());
                LuaEquilibria &eqs   = *hEqs;
                Lua::State    &L     = *eqs.lvm;
                const bool     isLua = isalpha(k[1]);
                Equilibrium   &eq    = (isLua ? eqs.makeLua(name,k) : eqs.make(name, L.eval<lua_Number>(k)) );

                for(const Actor *a = p->head;a;a=a->next) eq(int(a->nu),a->sp);
                for(const Actor *a = r->head;a;a=a->next) eq(-int(a->nu),a->sp);
            }

            inline void onCHEMICAL(const size_t) noexcept
            {
                clearState();
            }

        };



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
            Y_Jive_OnInternal(Compiler,CHEMICAL);
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

