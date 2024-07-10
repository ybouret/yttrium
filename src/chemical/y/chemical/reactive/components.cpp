

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
                throw;
            }

            updateKind();
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
            xmul << mOne;
            prod.massAction(xmul,C,level);
            const xreal_t lhs = xmul.product();
            return rhs + lhs;
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
            xmul << mOne;
            prod.massAction(xmul,C0,xi,level);
            const xreal_t lhs = xmul.product();
            return rhs + lhs;
        }

        void Components:: moveSafe(XWritable    &C,
                                   const xreal_t xi,
                                   const Level   level) const
        {
            reac.moveSafe(C,-xi,level);
            prod.moveSafe(C,xi,level);
        }

        void Components:: drvsMassAction(const xreal_t     K,
                                         XWritable  &      phi,
                                         const Level       output,
                                         const XReadable & C,
                                         const Level       input,
                                         XMul             &xmul) const
        {
            phi.ld(zero);
            xmul.free();
            reac.drvsMassAction(phi, output, K,    xmul, C, input);
            prod.drvsMassAction(phi, output, mOne, xmul, C, input);
        }


        bool Components:: linkedTo(const Species &sp) const noexcept
        {
            return 0 != db.search(sp.name);
        }

        bool Components:: linkedTo(const Components &other) const noexcept
        {
            size_t            n  = db.size();
            for(ConstIterator it = db.begin();n>0;--n,++it)
            {
                if( other.linkedTo( (**it).sp ) ) return true;
            }
            return false;
        }

        void Components:: record(AddressBook &book) const
        {
            reac.record(book);
            prod.record(book);
        }


    }

}


