

#include "y/chemical/reactive/components.hpp"
#include "y/system/exception.hpp"
#include "y/text/plural.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Components:: ~Components() noexcept
        {}

        Components::ConstInterface & Components:: surrogate() const noexcept { return db; }

#define Y_ChemCompCat(NAME) case NAME : return #NAME

        const char *  CategoryText(const Category cat) noexcept
        {
            switch(cat)
            {
                    Y_ChemCompCat(Nebulous);
                    Y_ChemCompCat(ProdOnly);
                    Y_ChemCompCat(ReacOnly);
                    Y_ChemCompCat(Standard);
            }
            return Core::Unknown;
        }

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
                                        const Level      L) const
        {
            xmul.free();

            assert(xmul.isEmpty());
            xmul << K;
            reac.massAction(xmul,C,L);
            const xreal_t rhs = xmul.product();

            assert(xmul.isEmpty());
            xmul << mOne;
            prod.massAction(xmul,C,L);
            const xreal_t lhs = xmul.product();
            
            return rhs + lhs;
        }

        real_t Components:: affinity(const xreal_t    K,
                                     XMul            &xmul,
                                     const XReadable &C,
                                     const Level      L) const
        {
            xmul.free();

            assert(xmul.isEmpty());
            xmul << K;
            reac.massAction(xmul,C,L);
            const xreal_t num = xmul.product();

            assert(xmul.isEmpty());
            xmul << pOne;
            prod.massAction(xmul,C,L);
            const xreal_t den = xmul.product();


            const xreal_t K_Over_Q = num/den;
            //std::cerr << "Num=" << num << " / Den=" << den << " => " << K_Over_Q << std::endl;
            return K_Over_Q.log();

        }


        xreal_t Components:: massAction(const xreal_t    K,
                                        XMul            &xmul,
                                        const XReadable &C,
                                        const Level      L,
                                        const xreal_t    xi) const
        {
            xmul.free();
            assert(xmul.isEmpty());

            xmul << K;
            reac.massAction(xmul,C,L,-xi);
            const xreal_t rhs = xmul.product();
            

            assert(xmul.isEmpty());
            xmul << mOne;
            prod.massAction(xmul,C,L,xi);
            const xreal_t lhs = xmul.product();
            //std::cerr << "[reac=" << rhs << ", prod=" << lhs << ",xi=" << real_t(xi) << "]" << std::endl;
            return rhs + lhs;
        }

        void Components:: moveSafe(XWritable    &C,
                                   const Level   L,
                                   const xreal_t xi) const
        {
            reac.moveSafe(C,L,-xi);
            prod.moveSafe(C,L, xi);
        }


        //! Cout = (Cinp + xi * nu) >= 0
        void Components:: addSafe(XWritable       &Cout,
                                  const Level      Lout,
                                  const XReadable &Cinp,
                                  const Level     &Linp,
                                  const xreal_t    xi) const
        {
            reac.addSafe(Cout, Lout, Cinp, Linp, -xi);
            prod.addSafe(Cout, Lout, Cinp, Linp,  xi);
        }


        void Components:: drvsMassAction(const xreal_t     K,
                                         XWritable  &      drvs,
                                         const Level       Lout,
                                         const XReadable & Cinp,
                                         const Level       Linp,
                                         XMul             &xmul) const
        {
            drvs.ld(zero);
            xmul.free();
            //std::cerr << "drvs@" << Cinp << std::endl;
            reac.drvsMassAction(drvs, Lout, K,    xmul, Cinp, Linp);
            prod.drvsMassAction(drvs, Lout, mOne, xmul, Cinp, Linp);
        }


        void Components:: drvsMassAction(const xreal_t     K,
                                         XWritable       & drvs,
                                         XWritable       & Cout,
                                         const Level       Lout,
                                         XMul            & xmul,
                                         const XReadable & Cinp,
                                         const Level       Linp,
                                         const xreal_t     xi) const
        {
            drvs.ld(zero);
            xmul.free();
            reac.drvsMassAction(drvs, Cout, Lout, K,    xmul, Cinp, Linp, -xi);
            prod.drvsMassAction(drvs, Cout, Lout, mOne, xmul, Cinp, Linp,  xi);
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


        bool Components:: neutral() const noexcept
        {
            return 0 == reac.charge() - prod.charge();
        }


        void Components:: viz(OutputStream &fp,
                              const String &color) const
        {
            Node(fp,this) << '[';
            Label(fp,name);
            fp << ",color=" << color << ",fontcolor=" << color;
            fp << ",shape=box";
            fp << ']';
            Endl(fp);
            for(const Actor *a=reac->head;a;a=a->next) a->viz(fp, *this, color, Iterating::Forward);
            for(const Actor *a=prod->head;a;a=a->next) a->viz(fp, *this, color, Iterating::Reverse);
        }


        xreal_t Components:: dot(const XReadable & array,
                                 const Level       level,
                                 XAdd &            xadd) const
        {
            size_t n = db.size();
            xadd.make(n); assert(xadd.isEmpty());
            for(ConstIterator it=db.begin();n>0;--n,++it)
            {
                const Component &cc = **it;
                const xreal_t    xm = array[cc.sp.indx[level]] * cc.xn;
                xadd << xm;
            }
            return xadd.sum();
        }

        xreal_t Components:: dot(const XReadable &lhs,
                                 const XReadable &rhs,
                                 const Level      level,
                                 XAdd            &xadd) const
        {
            size_t n = db.size();
            xadd.make(n); assert(xadd.isEmpty());
            for(ConstIterator it=db.begin();n>0;--n,++it)
            {
                const Component &cc = **it;
                const size_t     ii = cc.sp.indx[level];
                const xreal_t    xm = lhs[ii] * rhs[ii];
                xadd << xm;
            }
            return xadd.sum();
        }

        const char * Components:: kindText() const noexcept
        {
            return CategoryText(kind);
        }

        void Components:: mustSupport(const XReadable &C, const Level L) const
        {
            if( reac.deficient(C,L) ) throw Specific::Exception( name.c_str(), "deficient in reactant%s", Plural::s(reac->size) );
            if( prod.deficient(C,L) ) throw Specific::Exception( name.c_str(), "deficient in product%s", Plural::s(prod->size) );
        }


    }

}


