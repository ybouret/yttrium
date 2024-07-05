

#include "y/chemical/reactive/components.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Components:: ~Components() noexcept
        {}

        Components::ConstInterface & Components:: surrogate() const noexcept { return db; }

        const char * const Components:: Mark = "<=>";

        void Components:: updateKind() noexcept
        {
            if(reac->size<=0)
            {
                if(prod->size<=0)
                {
                    Coerce(kind) = Nebulous;;
                }
                else
                {
                    Coerce(kind) = ProdOnly;;
                }
            }
            else
            {
                assert(reac->size>0);
                if(prod->size<=0)
                {
                    Coerce(kind) = ReacOnly;
                }
                else
                {
                    Coerce(kind) = Standard;
                }
            }
        }


        void Components:: operator()(const int      nu,
                                     const Species &sp)
        {

            const String &uid = sp.name;
            if(db.search(uid)) throw Specific::Exception(name.c_str(), "mutliple species '%s'", uid.c_str());

            Actors   *ac = 0;
            unsigned  nn = 0;
            switch( Sign::Of(nu) )
            {
                case __Zero__: throw Specific::Exception(name.c_str(), "zero stoichiometry for '%s'", uid.c_str());
                case Positive: ac = & Coerce(prod); nn=nu;  break;
                case Negative: ac = & Coerce(reac); nn=-nu; break;
            }
            assert(0!=ac);
            assert(nn>0);
            {
                const Component::Ptr comp = new Component(nu,sp);
                if(!db.insert(comp)) throw Specific::Exception(name.c_str(), "unexpected failure to use '%s'", uid.c_str());
            }

            try
            {
               (*ac)(nn,sp);
            }
            catch(...)
            {
                (void) db.remove(uid);
            }

            


        }

        xreal_t Components:: massAction(const xreal_t    K,
                                        XMul            &xmul,
                                        const XReadable &C,
                                        const Level      level) const
        {
            xmul.free();
            xmul << K;
            reac.massAction(xmul,C,level);
            const xreal_t rhs = xmul.product();

            assert(xmul.isEmpty());
            xmul << 1;
            prod.massAction(xmul,C,level);
            const xreal_t lhs = xmul.product();
            return rhs - lhs;
        }

        xreal_t Components:: massAction(const xreal_t    K,
                                        XMul            &xmul,
                                        const XReadable &C0,
                                        const xreal_t    xi,
                                        const Level      level) const
        {
            xmul.free();
            xmul << K;
            reac.massAction(xmul,C0,-xi,level);
            const xreal_t rhs = xmul.product();

            assert(xmul.isEmpty());
            xmul << 1;
            prod.massAction(xmul,C0,xi,level);
            const xreal_t lhs = xmul.product();
            return rhs - lhs;
        }




    }

}


