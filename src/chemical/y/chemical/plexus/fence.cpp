#include "y/chemical/plexus/fence.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Fence:: ~Fence() noexcept {}

        Fence:: Fence(Banks &banks) noexcept :
        capping(banks.hb,banks.ls),
        missing(banks.hb,banks.ls),
        cursor(0),
        zeroed(banks.ls),
        zero(0)
        {
        }

        Fence:: Fence(const Fence &other) :
        capping(other.capping),
        missing(other.missing),
        cursor(other.cursor),
        zeroed(other.zeroed),
        zero(other.zero)
        {
        }

        std::ostream & operator<<(std::ostream &os, const Fence &self)
        {
            os << "capping=" << self.capping << " / missing=" << self.missing;
            return os;
        }

        void Fence:: reset() noexcept {
            Coerce(capping).reset();
            Coerce(missing).reset();
            Coerce(cursor) = 0;
            Coerce(zeroed).free();
        }


#define Y_CHEM_FENCE(ST) case ST: return #ST

        const char * Fence:: StatusText(const unsigned flags) noexcept
        {
            switch(flags)
            {
                    Y_CHEM_FENCE(RUNNING);
                    Y_CHEM_FENCE(BLOCKED);
                    Y_CHEM_FENCE(PARTIAL);
                    Y_CHEM_FENCE(EQUATED);
                default:
                    break;
            }
            return Core::Unknown;
        }


        void Fence:: buildLimits(const Components &components,
                                 const XReadable  &C)
        {
            //--------------------------------------------------------------
            //
            // initialize
            //
            //--------------------------------------------------------------
            reset();
            assert(components.reac.size>0);
            assert(components.prod.size>0);

            //--------------------------------------------------------------
            //
            // scan reactants
            //
            //--------------------------------------------------------------
            for(const Actor *a=components.reac.head;a;a=a->next)
            {
                const Species &sp = a->sp;
                const xreal_t  nu = a->xnu;
                const xreal_t  cc = C[sp.indx[TopLevel]];
                if(cc>=zero)
                {
                    Coerce(capping).reac(sp,cc/nu);
                }
                else
                {
                    Coerce(missing).reac(sp,(-cc)/nu);
                }
            }

            //--------------------------------------------------------------
            //
            // scan products
            //
            //--------------------------------------------------------------
            for(const Actor *a=components.prod.head;a;a=a->next)
            {
                const Species &sp = a->sp;
                const xreal_t  nu = a->xnu;
                const xreal_t  cc = C[sp.indx[TopLevel]];
                if(cc>=zero)
                {
                    Coerce(capping).prod(sp,cc/nu);
                }
                else
                {
                    Coerce(missing).prod(sp,(-cc)/nu);
                }
            }

        }


        void Fence:: startUpWith(const Boundary &             bnd,
                                 const Equilibrium::Direction dir)
        {
            assert(bnd.xi>=zero);
            switch(dir)
            {
                case Equilibrium::Forward: Coerce(cursor) =  bnd.xi; break;
                case Equilibrium::Reverse: Coerce(cursor) = -bnd.xi; break;
            }
            Coerce(zeroed) << *bnd;
            assert(zeroed.size==bnd->size);
        }

        void Fence:: proceedWith(const Boundary &bnd)
        {
            Coerce(zeroed) << *bnd;
        }


        unsigned Fence:: operator()(const Components &components,
                                    const XReadable  &C,
                                    XMLog            &xml)
        {
            buildLimits(components,C);

            //------------------------------------------------------------------
            //
            //
            // local definitions
            //
            //
            //------------------------------------------------------------------
            static const unsigned MISSING_NONE = 0x00;
            static const unsigned MISSING_REAC = 0x01;
            static const unsigned MISSING_PROD = 0x02;
            static const unsigned MISSING_BOTH = MISSING_REAC|MISSING_PROD;;

            //------------------------------------------------------------------
            //
            //
            // initialize
            //
            //
            //------------------------------------------------------------------
            unsigned                 flag  = MISSING_NONE;
            if(missing.reac->size>0) flag |= MISSING_REAC;
            if(missing.prod->size>0) flag |= MISSING_PROD;

            Y_XMLOG(xml, " (@) " << *this);

            //------------------------------------------------------------------
            //
            //
            // process cases
            //
            //
            //------------------------------------------------------------------
            switch(flag)
            {
                    //----------------------------------------------------------
                    //
                case MISSING_NONE:
                    Y_XMLOG(xml, " (-) missing none");
                    return RUNNING;
                    //----------------------------------------------------------

                    //----------------------------------------------------------
                    //
                    //
                case MISSING_REAC: return missingReac(xml);
                    //
                    //----------------------------------------------------------


                    //----------------------------------------------------------
                    //
                case MISSING_PROD: return missingProd(xml);
                    //
                    //----------------------------------------------------------

                    //----------------------------------------------------------
                    //
                case MISSING_BOTH: Y_XMLOG(xml, " (+) missing both"); default:
                    //
                    //----------------------------------------------------------
                    break;
            }
            assert(MISSING_BOTH==flag);
            return BLOCKED | BY_BOTH;
        }


        unsigned Fence:: missingProd(XMLog &xml)
        {
            static const char fn[] = "Chemical::Fence::missingProd";

            assert(0==missing.reac->size); 
            assert(missing.prod->size>0);
            assert(capping.reac->size>0);

            const Boundary & bad = **(missing.prod->head); // bad is first prod
            const Boundary & dom = **(capping.reac->head); // dom is first reac
            Y_XMLOG(xml, " (+) missing  prod=" << bad);
            Y_XMLOG(xml, " (+) dominant reac=" << dom );

            assert(bad.xi>zero);
            if(dom.xi<=zero) {
                Y_XMLOG(xml, " (*) blocked");
                return BLOCKED | BY_REAC;
            }

            switch( Sign::Of(dom.xi,bad.xi) )
            {
                case Negative:
                    assert(dom.xi<bad.xi);
                    startUpWith(dom,Equilibrium::Forward);
                    Y_XMLOG(xml, " (*) partial  reac=" << zeroed << "@" << real_t(cursor) );
                    assert(cursor>zero);
                    return PARTIAL | BY_REAC;

                case __Zero__:
                    startUpWith(bad,Equilibrium::Forward);
                    proceedWith(dom);
                    Y_XMLOG(xml, " (*) equated  both=" << zeroed << "@" << real_t(cursor) );
                    assert(cursor>zero);
                    return EQUATED | BY_BOTH;

                case Positive:
                    assert(dom.xi>bad.xi);
                    startUpWith(bad,Equilibrium::Forward);
                    Y_XMLOG(xml, " (*) equated  prod=" << zeroed << "@" << real_t(cursor) );
                    assert(cursor>zero);
                    return EQUATED | BY_PROD;
            }


            throw Specific::Exception(fn, "corrupted signs");
            return -1;
        }

        unsigned Fence:: missingReac(XMLog &xml)
        {
            static const char fn[] = "Chemical::Fence::missingReac";

            assert(0==missing.prod->size);
            assert(missing.reac->size>0);
            assert(capping.prod->size>0);

            const Boundary & bad =  **(missing.reac->head); // bad is first reactant
            const Boundary & dom =  **(capping.prod->head); // dom is first product
            Y_XMLOG(xml, " (+) missing  reac=" << bad);
            Y_XMLOG(xml, " (+) dominant prod=" << dom );
            assert(bad.xi>zero);
            if(dom.xi<=zero) {
                Y_XMLOG(xml, " (*) blocked");
                return BLOCKED | BY_PROD;
            }

            switch( Sign::Of(dom.xi,bad.xi) )
            {
                case Negative:
                    assert(dom.xi<bad.xi);
                    startUpWith(dom,Equilibrium::Reverse);
                    Y_XMLOG(xml, " (*) partial  prod=" << zeroed << "@" << real_t(cursor) );
                    assert(cursor<=zero);
                    return PARTIAL | BY_PROD;

                case __Zero__:
                    startUpWith(bad,Equilibrium::Reverse);
                    proceedWith(dom);
                    Y_XMLOG(xml, " (*) equated  both=" << zeroed << "@" << real_t(cursor) );
                    assert(cursor<=zero);
                    return EQUATED | BY_BOTH;

                case Positive:
                    assert(dom.xi>bad.xi);
                    startUpWith(bad,Equilibrium::Reverse);
                    Y_XMLOG(xml, " (*) equated  reac=" << zeroed << "@" << real_t(cursor) );
                    assert(cursor<=zero);
                    return EQUATED | BY_REAC;
            }
            throw Specific::Exception(fn, "corrupted signs");
            return -1;
        }


    }

}

