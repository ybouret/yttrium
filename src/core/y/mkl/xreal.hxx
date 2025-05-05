
namespace Yttrium
{
    template <> XReal<real_t>:: ~XReal() noexcept {}

    template <> XReal<real_t>:: XReal() noexcept :
    exponent(0), mantissa(0)
    {
    }
    
    template <> XReal<real_t>:: XReal(const XReal &_) noexcept :
    exponent(_.exponent), 
    mantissa(_.mantissa)
    {
    }

    template <> XReal<real_t> & XReal<real_t>:: operator=(const XReal<real_t> &_) noexcept
    {
        Coerce(exponent) = _.exponent;
        Coerce(mantissa) = _.mantissa;
        return *this;
    }

    template <>
    void XReal<real_t> :: neg() noexcept
    {
        switch( Sign::Of(mantissa) )
        {
            case Negative:
            case Positive:
                Coerce(mantissa) = -mantissa;
                break;
            case __Zero__:
                assert(0==exponent);
                break;
        }
    }

    template <>
    void XReal<real_t> :: ldz() noexcept
    {
        Coerce(exponent) = 0;
        Coerce(mantissa) = 0;
    }


    template <> XReal<real_t>:: XReal(const real_t x)  :
    exponent(0), mantissa( std::frexp(x,&Coerce(exponent)) )
    {

    }

    template <> XReal<real_t>:: XReal(const real_t x, const Raised_ &, const int xp) :
    exponent(0), mantissa( std::frexp(x,&Coerce(exponent)) )
    {
        Coerce(exponent) += xp;
    }

    template <>
    XReal<real_t> & XReal<real_t>:: operator=(const real_t x)
    {
        const XReal<real_t> _(x);
        Coerce(exponent) =  _.exponent;
        Coerce(mantissa) =  _.mantissa;
        return *this;
    }

    template <>
    XReal<real_t> XReal<real_t>::Mul(const XReal &lhs, const XReal &rhs) noexcept
    {
        const real_t lm = lhs.mantissa;
        if( std::fabs(lm) <= 0)
        {
            return XReal();
        }
        else
        {
            const real_t rm = rhs.mantissa;
            if( std::fabs(rm) <= 0 )
            {
                return XReal();
            }
            else
            {
                const int   xp = lhs.exponent + rhs.exponent;
                const XReal xr(lm*rm);
                Coerce(xr.exponent) += xp;
                return xr;
            }
        }
    }

    template <>
    XReal<real_t>:: operator real_t() const
    { return std::ldexp(mantissa,exponent); }


    template <>
    void XReal<real_t>:: display(std::ostream &os) const
    {
        static const unsigned radix = MKL::Numeric<real_t>::RADIX;
        switch( XRealOutput::Mode )
        {
            case XRealOutput::Default:
                os << '(' << mantissa;
                switch(exponent)
                {
                    case -1: os << "/" << radix; break;
                    case  0: break;
                    case  1: os << "*" << radix; break;
                    default:
                        if(exponent>0)
                            os << "*" << radix << "^" << exponent;
                        else
                            os << "/" << radix << "^" << -exponent;
                }
                os << ')';
                break;

            case XRealOutput::Compact:
                os << static_cast<real_t>(*this);
                break;
        }
    }



    template<>
    XReal<real_t>:: XReal(const int e, const real_t m) noexcept :
    exponent(e),
    mantissa(m)
    {
    }


    template <>
    XReal<real_t> XReal<real_t>:: abs() const noexcept
    {
        switch( Sign::Of(mantissa) )
        {
            case Negative: return XReal(exponent,-mantissa);
            case Positive: return *this;
            case __Zero__: break;
        }
        return XReal();
    }

    template <>
    XReal<real_t>  XReal<real_t>:: Div(const XReal &numer,
                                       const XReal &denom)
    {
        if( std::fabs(denom.mantissa) <= 0)
            throw Libc::Exception(EDOM,"XReal division by zero");

        if( std::fabs(numer.mantissa) <=0 )
            return XReal();

        const int   xp = numer.exponent - denom.exponent;
        const XReal xr(numer.mantissa/denom.mantissa);
        Coerce(xr.exponent) += xp;
        return xr;
    }

    template <>
    XReal<real_t> XReal<real_t>:: operator+() const noexcept
    {
        return *this;
    }




    // big.mantissa * radix^big.exponent + lit.mantissa * radix^lit.mantissa
    // = radix^big.exponent * ( big.mantissa + lit.mantissa * radix ^(lit.exponent-big.exponent) )
    static inline
    XReal<real_t> __add(const XReal<real_t> big,
                        const XReal<real_t> lit) noexcept
    {

        assert(big.exponent>lit.exponent);
        const int difExponent = lit.exponent - big.exponent;
        if(difExponent < MKL::Numeric<real_t>::MIN_EXP)
        {
            return big;
        }
        else
        {
            const  XReal<real_t> xr( big.mantissa + std::ldexp(lit.mantissa,difExponent) );
            Coerce(xr.exponent) += big.exponent;
            return xr;
        }
    }

    template <>
    XReal<real_t> XReal<real_t>:: Add(const XReal<real_t> &lhs, const XReal<real_t> &rhs)
    {
        if( std::fabs(lhs.mantissa) <= 0) return rhs;
        if( std::fabs(rhs.mantissa) <= 0) return lhs;

        assert(std::fabs(lhs.mantissa)>0);
        assert(std::fabs(rhs.mantissa)>0);

        // check action depending on difference of exponents
        switch( Sign::Of(lhs.exponent,rhs.exponent) )
        {
            case Negative: assert(lhs.exponent<rhs.exponent); return __add(rhs,lhs);
            case Positive: assert(lhs.exponent>rhs.exponent); return __add(lhs,rhs);
            case __Zero__: break;
        }

        // same exponent
        const real_t m = lhs.mantissa + rhs.mantissa;
        if( std::fabs(m) <= 0)
            return XReal(); // zero
        else
        {
            const int   comExponent = lhs.exponent; assert(rhs.exponent==comExponent);
            const  XReal xr(m);
            return XReal(xr.exponent+comExponent,xr.mantissa);
        }
    }


    template <>
    XReal<real_t> XReal<real_t>:: operator-() const noexcept
    {
        return XReal(exponent,-mantissa);
    }

    template <>
    XReal<real_t> XReal<real_t>:: Sub(const XReal<real_t> &lhs, const XReal<real_t> &rhs)
    {
        const XReal<real_t> r = -rhs;
        return Add(lhs,r);
    }

    template <>
    XReal<real_t> XReal<real_t>:: sqrt() const
    {
        switch( Sign::Of(mantissa) )
        {
            case Negative: throw Libc::Exception(EDOM,"XReal negative square root");
            case __Zero__: return XReal();
            case Positive:
                break;
        }
        real_t m = mantissa;
        int    p = exponent;
        if( 0!= (0x1&p) )
        {
            m *= MKL::Numeric<real_t>::RADIX;
            --p;
        }
        assert(0==(0x01&p));
        const XReal s = std::sqrt(m);
        return XReal(s.exponent+p/2,s.mantissa);
    }



    template <>
    SignType XReal<real_t>:: Compare(const XReal<real_t> &lhs, const XReal<real_t> &rhs) noexcept
    {
        const XReal delta = Sub(lhs,rhs);
        return Sign::Of(delta.mantissa);
    }

    template <>
    SignType XReal<real_t>:: Compare(const XReal<real_t> &lhs, const  real_t rhs) noexcept
    {
        const XReal R(rhs);
        return Compare(lhs,R);
    }

    template <>
    SignType XReal<real_t>:: Compare(const  real_t lhs, const XReal<real_t> &rhs) noexcept
    {
        const XReal L(lhs);
        return Compare(L,rhs);
    }

    template <>
    real_t XReal<real_t>:: log10() const
    {
        static const real_t l10r = std::log10(  MKL::Numeric<real_t>::RADIX );
        if(mantissa<=0) throw Libc::Exception(EDOM, "XReal::log10()");
        return std::log10(mantissa) + exponent * l10r;
    }

    template <>
    real_t XReal<real_t>:: p() const
    {
        return -this->log10();
    }

    template <>
    real_t XReal<real_t>:: log() const
    {
        static const real_t lnr = std::log(  MKL::Numeric<real_t>::RADIX );
        if(mantissa<=0) throw Libc::Exception(EDOM, "XReal::log()");
        return std::log(mantissa) + exponent * lnr;
    }



    template <>
    XReal<real_t> XReal<real_t>::  pow(const real_t alpha) const
    {
        static const real_t  radix = MKL::Numeric<real_t>::RADIX;
        if(mantissa<0) throw Libc::Exception(EDOM, "XReal::pow()");

        // (*) (mantissa*r^exponent)^alpha = mantissa^alpha * r*(exponent*alpha)
        // (*) exponent*alpha = ip + fp
        const XReal<real_t> ma  = std::pow(mantissa,alpha);
        const real_t        xp  = alpha * exponent;
        const real_t        ip  = std::floor(xp);
        const real_t        fp  = xp-ip;
        const XReal<real_t> ra  = std::pow(radix,fp);
        const XReal<real_t> ans = ra * ma;
        Coerce(ans.exponent) += static_cast<int>(ip);
        return ans;
    }

    template<>
    String XReal<real_t>:: str() const
    {
        return Formatted::Get("%.15g",double( static_cast<real_t>(*this)));
    }

    template <>
    String XReal<real_t>:: ToString(const XReal &xr)
    {
        return xr.str();
    }


}

