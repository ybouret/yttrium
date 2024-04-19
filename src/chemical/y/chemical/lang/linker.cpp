
#include "y/chemical/lang/linker.hpp"
#include "y/jive/syntax/xnode.hpp"
#include "y/jive/syntax/rule.hpp"
#include "y/type/temporary.hpp"
#include "y/system/exception.hpp"
#include "y/container/algo/crop.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Linker:: Players::  Players() noexcept : Object(), Actors(), next(0), prev(0) {}
        Linker:: Players:: ~Players() noexcept {}


        const char * Linker:: callSign() const noexcept {
            assert(0!=call);
            return call;
        }

        Linker:: ~Linker() noexcept {}

        Linker:: Linker( const char * const id)   :
        Jive::Syntax::Translator(),
        call(id),
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
            Y_Jive_OnTerminal(Linker,UUID);
            forTerminal('+', *this, &Linker::onPLUS);
            forTerminal('-', *this, &Linker::onMINUS);
            Y_Jive_OnInternal(Linker,ZPOS);
            Y_Jive_OnInternal(Linker,ZNEG);
            Y_Jive_OnInternal(Linker,SPECIES);
            Y_Jive_OnTerminal(Linker,COEF);
            Y_Jive_OnInternal(Linker,ACTOR);
            Y_Jive_OnInternal(Linker,REAC);
            Y_Jive_OnInternal(Linker,PROD);
            Y_Jive_OnTerminal(Linker,K);
            Y_Jive_OnInternal(Linker,EQ);
            Y_Jive_OnTerminal(Linker,RXP);
            Y_Jive_OnInternal(Linker,CHEMICAL);
        }


        void Linker:: release() noexcept
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


        static inline bool IsPLUS(const Jive::Syntax::XNode &node) noexcept
        {
            return '+' == *(node.rule.name);
        }

        static inline void PreProcess(Jive::Syntax::XNode &node, void *, const int) noexcept
        {
            const String &id = *(node.rule.name);
            if( "REAC" == id  || "PROD" == id)
                node.removeChildIf(IsPLUS);
        }


        void Linker:: operator()(Jive::Syntax::XNode &    tree,
                                 Library &                lib,
                                 LuaEquilibria &          eqs,
                                 Sequence<String> * const rxp)
        {
            release();
            try
            {
                //--------------------------------------------------------------
                // preprocess tree
                //--------------------------------------------------------------
                apply(tree,PreProcess,0);

                //--------------------------------------------------------------
                // set global variables
                //--------------------------------------------------------------
                const Temporary<Library *>          tlib(hLib,&lib); // set lib
                const Temporary<LuaEquilibria *>    teqs(hEqs,&eqs); // set eqs
                const Temporary<Sequence<String> *> trxp(hRxp,rxp);  // set rxp

                //--------------------------------------------------------------
                // translate
                //--------------------------------------------------------------
                //translate(tree,Jive::Syntax::Permissive);
                translate(tree,Jive::Syntax::Restricted);

            }
            catch(...)
            {
                release();
                throw;
            }
        }


        void Linker:: onUUID(  const Jive::Token &token) { uuid << token.toString(); }
        void Linker:: onPLUS(  const Jive::Token &)      {  }
        void Linker:: onMINUS( const Jive::Token &)      {  }
        void Linker:: onZPOS(size_t n) { assert(n>0); algZ <<  int(n); }
        void Linker:: onZNEG(size_t n) { assert(n>0); algZ << -int(n); }

        //! create/query species with/without charge from library
        void Linker:: onSPECIES(const size_t n)
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

        void Linker:: onCOEF(const Jive::Token &token)
        {
            apn nu = 0;
            for(const Jive::Char *ch=token.head;ch;ch=ch->next)
            {
                const char code = **ch; assert(code>='0'); assert(code<='9');
                nu *= 10;
                nu += code - '0';
            }
            if(nu<=0) throw Specific::Exception(callSign(), "zero stoichiometry");
            coef << nu.cast<int>("nu");
        }

        void Linker:: onACTOR(const size_t n)
        {
            assert( (1==n) || (2==n) );
            const Species &sp = spec.pullTail();
            unsigned       nu = 1;
            if(2==n)
                nu = coef.pullTail();
            cast.pushTail( new Actor(nu,sp) );

        }

        void Linker:: gather(Players * const target, size_t n) noexcept
        {
            assert(0!=target);
            while(n-- > 0 ) target->pushHead( cast.popTail() );
        }

        void Linker:: onREAC(const size_t n)
        {
            gather( reac.pushTail( new Players() ), n);
        }

        void Linker:: onPROD(const size_t n)
        {
            gather( prod.pushTail( new Players() ), n);
        }

        void Linker:: onK(const Jive::Token &token)
        {
            String k = token.toString(1,1);
            Algo::Crop(k,isspace);
            Kstr << k;
        }

        void Linker:: onEQ(const size_t)
        {
            //assert(4==n);
            const String           name = uuid.pullTail();
            const AutoPtr<Players> r    = reac.popTail();
            const AutoPtr<Players> p    = prod.popTail();
            const String           k    = Kstr.pullTail();

            if(k.size()<=0) throw Specific::Exception(callSign(),"<%s> empty constant string", name.c_str());
            LuaEquilibria &eqs   = *hEqs;
            Lua::State    &L     = *eqs.lvm;
            const bool     isLua = isalpha(k[1]);
            Equilibrium   &eq    = (isLua ? eqs.makeLua(name,k) : eqs.make(name, L.eval<lua_Number>(k)) );

            for(const Actor *a = p->head;a;a=a->next) eq(int(a->nu),a->sp);
            for(const Actor *a = r->head;a;a=a->next) eq(-int(a->nu),a->sp);
        }

        void Linker:: onRXP(const Jive::Token &token)
        {
            const String rx = token.toString(1,0);
            std::cerr << "RXP='" << rx << "'" << std::endl;
            if(hRxp) 
                hRxp->pushTail(rx);
            else
                throw Specific::Exception(callSign(),"unauthorized regular expression (@%s)",rx.c_str());
        }

        void Linker:: onCHEMICAL(const size_t) noexcept
        {
            release();
        }

    }

}
