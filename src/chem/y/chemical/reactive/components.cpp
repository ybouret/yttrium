#include "y/chemical/reactive/components.hpp"
#include "y/system/exception.hpp"
#include <cerrno>
#include "y/kemp/integer.hpp"

namespace Yttrium
{
    namespace Chemical
    {

#define Y_ChemCm(PPTY) case PPTY: return #PPTY

        const char * Components:: AttributeText(const Attribute p) noexcept
        {
            switch(p)
            {
                    Y_ChemCm(Nebulous);
                    Y_ChemCm(ReacOnly);
                    Y_ChemCm(ProdOnly);
                    Y_ChemCm(Definite);
            }
            return Core::Unknown;
        }

        Components:: ~Components() noexcept
        {
        }

        Components:: ConstInterface & Components:: surrogate() const noexcept { return cmdb; }


        const String & Components:: key() const noexcept
        {
            return name;
        }

        Actors & Components:: actorsPlaying(const Role role)
        {
            switch(role)
            {
                case Reactant: return Coerce(reac);
                case Product:  return Coerce(prod);
            }
            throw Libc::Exception(EINVAL, "invalid Chemical::Role!!");
            // never get here
        }

        void Components:: operator()(const Role role, const unsigned nu, const Species &sp)
        {
            const String &sid = sp.name;

            // check coef
            if(nu<=0) throw Specific::Exception(name.c_str(), "null stoichiometry for '%s'", sid.c_str());

            // check that species is not used
            {
                const Component * const cm = cmdb.search(sid);
                if(0!=cm)
                    throw Specific::Exception(name.c_str(), "already used as %s %s", cm->actor.name.c_str(), cm->side());
            }

            // get concerned actors
            Actors &      actors = actorsPlaying(role);
            Actors        backup = actors;

            // create new actor
            const Actor & actor  = actors(nu,sp);

            // safe insertion
            try {
                const Component cm(actor,role);
                if(!cmdb.insert(cm))
                    throw Specific::Exception(name.c_str(),"couldn't insert %s %s", cm.actor.name.c_str(), cm.side());
            }
            catch(...)
            {
                actors.company.xch(backup.company);
                throw;
            }

            switch(role)
            {
                case Product:  Coerce(d_nu) += int(nu); break;
                case Reactant: Coerce(d_nu) -= int(nu); break;
            }

            updateAttribute();
        }

        void Components:: operator()(const Role role, const Species &sp)
        {
            (*this)(role,1,sp);
        }


        void Components:: updateAttribute() noexcept
        {
            if(reac->size>0)
            {
                if(prod->size>0)
                {
                    Coerce(attr) = Definite;
                }
                else
                {
                    Coerce(attr) = ReacOnly;
                }
            }
            else
            {
                if(prod->size>0)
                {
                    Coerce(attr) = ProdOnly;
                }
                else
                {
                    Coerce(attr) = Nebulous;
                }
            }
        }

        void Components :: viz(OutputStream &fp,
                               const char * const color,
                               const char * const style) const
        {
            Node(fp,this) << '[';
            Label(fp,name) << ",shape=box3d";
            if(color) fp << ",color=" << color << ",fontcolor=" << color;
            if(style) fp << "style=\"" << style << "\"";
            Endl(fp<<']');
            
            for(ConstIterator it=cmdb.begin();it!=cmdb.end();++it)
            {
                const Component &cm = *it;
                cm.viz(fp,*this,color);
            }

        }

        void Components:: graphViz(OutputStream &fp) const
        {
            Enter(fp, "G");
            for(ConstIterator it=cmdb.begin();it!=cmdb.end();++it)
            {
                const Component &cm = *it;
                cm.actor.sp.viz(fp,0,0);
            }
            viz(fp,0,0);
            Leave(fp);
        }

        static inline void sumZ(apz &sum, const Actor *a)
        {
            for(;a;a=a->next)
            {
                apz z = a->sp.z;
                z *= a->nu;
                sum += z;
            }
        }

        bool Components:: neutral() const
        {
            apz lhs = 0; sumZ(lhs,reac->head);
            apz rhs = 0; sumZ(rhs,prod->head);
            return lhs == rhs;
        }

        void Components:: applicable() const
        {
            const char * const id = name.c_str();
            if(!neutral() )    throw Specific::Exception(id, "charge is not conserved");
            if(Nebulous==attr) throw Specific::Exception(id, "no components");
        }


        xReal Components:: activity(XMul &xmul, const xReal K, const XReadable &C, const Level L) const
        {
            xmul.free();
            xmul << K;
            reac.activity(xmul,C,L);
            const xReal ra = xmul.product();

            assert(xmul.isEmpty());
            xmul << one;
            prod.activity(xmul, C, L);
            const xReal pa = xmul.product();
            return ra-pa;
        }






        xReal Components:: prodActivity(XMul &xmul, const XReadable &C, const Level L, const xReal xi) const
        {
            xmul.free();
            xmul << one;
            prod.activity(xmul,C,L,xi);
            return -xmul.product();
        }

        xReal Components:: reacActivity(XMul &xmul, const xReal K, const XReadable &C, const Level L, const xReal xi) const
        {
            xmul.free();
            xmul << K;
            reac.activity(xmul,C,L,-xi);
            return xmul.product();
        }

        xReal Components:: activity(XMul &xmul, const xReal K, const XReadable &C, const Level L, const xReal xi) const
        {
            return reacActivity(xmul, K, C, L, -xi) + prodActivity(xmul, C, L, xi);
        }
        


        Situation Components:: situation(const XReadable &C, const Level L) const noexcept
        {
            if(reac.haveZero(C,L) && prod.haveZero(C,L) ) return Blocked;
            return Running;
        }


        const char * Components:: attrText() const noexcept
        {
            return AttributeText(attr);
        }



        SignType Components:: bracket(XTriplet &       xi,
                                      XTriplet &       ff,
                                      XMul     &       xmul,
                                      const xReal      K,
                                      const XReadable &C,
                                      const Level      L) const
        {
            assert( Running == situation(C,L) );
            const xReal zero;
            xi.a = xi.b = xi.c = zero;
            ff.a = ff.b = ff.c = activity(xmul, K, C, L);
            const SignType s = Sign::Of(ff.a);

            switch(s)
            {
                case __Zero__: break; // numeric zero
                case Positive: return positiveBracket(xi,ff,xmul,K,C,L);
                case Negative: return negativeBracket(xi,ff,xmul,K,C,L);
            }

            return s;
        }

        void Components:: moveSafely(XWritable &C, const Level L, const xReal xi) const noexcept
        {
            Coerce(prod).moveSafely(C,L,xi);
            Coerce(reac).moveSafely(C,L,-xi);
        }




        SignType Components:: positiveBracket(XTriplet &       xi,
                                              XTriplet &       ff,
                                              XMul    &        xmul,
                                              const xReal      K,
                                              const XReadable &C,
                                              const Level      L) const
        {
            assert(ff.a>0.0);
            // there is too much reactant/not enough product
            // forward xi>0 up to negative activity
            switch(attr)
            {
                case Nebulous:
                    throw Specific::Exception(name.c_str(),"no Nebulous positiveBracket");

                case ProdOnly:
                    // use saturating expression
                    ++Coerce( (xi.c = K.pow( 1.0/d_nu )).exponent );
                    assert( prodActivity(xmul, C, L, xi.c) < 0.0 );
                    break;

                case ReacOnly:
                case Definite:
                    // use limiting reactant
                    xi.c = reac.limitingExtent(C,L);
                    assert( prodActivity(xmul, C, L, xi.c) < 0.0 ); // -1 if reac only
                    break;

            }

            ff.c = prodActivity(xmul, C, L, xi.c); assert(ff.c<0.0);
            return Positive;
        }


        SignType Components:: negativeBracket(XTriplet &       xi,
                                              XTriplet &       ff,
                                              XMul    &        xmul,
                                              const xReal      K,
                                              const XReadable &C,
                                              const Level      L) const
        {
            assert(ff.a<0.0);
            // there is too much product/not enough reactant
            switch(attr)
            {
                case Nebulous:
                    throw Specific::Exception(name.c_str(),"no Nebulous positiveBracket");

                case ReacOnly:
                    // use saturation expression
                    ++Coerce( (xi.c = -K.pow( 1.0/-d_nu )).exponent );
                    assert(reacActivity(xmul, K, C, L, xi.c) > 0.0);
                    break;

                case Definite:
                case ProdOnly:
                    // use limiting product
                    xi.c = -prod.limitingExtent(C,L);
                    assert(reacActivity(xmul, K, C, L, xi.c) > 0.0);
                    break;
            }

            ff.c = reacActivity(xmul, K, C, L, xi.c); assert(ff.c>0.0);
            return Negative;
        }


        


        void Components:: addSpeciesTo(AddressBook &book) const
        {
            for(ConstIterator it=cmdb.begin();it!=cmdb.end();++it)
            {
                book |= (*it).actor.sp;
            }
        }

        const char * const Components::Scheme = "set18";

        String Components:: makeColor() const
        {
            return makeColorFrom(Scheme);
        }


    }

}

#include "y/chemical/weasel.hpp"

namespace Yttrium
{

    namespace Chemical
    {

        std::ostream & Components:: print(std::ostream &os) const
        {

            os << name << Weasel::COLON << reac << ' ' << Weasel::SYMBOL  << ' ' << prod;

            return os;
        }


    }

}

