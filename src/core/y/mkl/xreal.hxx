
namespace Yttrium
{
    template <> XReal<real_t>:: ~XReal() noexcept {}

    template <> XReal<real_t>:: XReal() noexcept :
    exponent(0), mantissa(0)
    {
    }
    
    template <> XReal<real_t>:: XReal(const XReal &_) noexcept :
    exponent(_.exponent), mantissa(_.mantissa)
    {
    }

    template <> XReal<real_t> & XReal<real_t>:: operator=(const XReal<real_t> &_) noexcept
    {
        Coerce(exponent) = _.exponent;
        Coerce(mantissa) = _.mantissa;
        return *this;
    }

    template <> XReal<real_t>:: XReal(const real_t x)  :
    exponent(0), mantissa( std::frexp(x,&Coerce(exponent)) )
    {

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
    void XReal<real_t>:: display(std::ostream &os) const
    {
        os << '(' << mantissa;
        switch(exponent)
        {
            case -1: os << "/2"; break;
            case  0: break;
            case  1: os << "*2"; break;
            default:
                if(exponent>0)
                    os << "*2^" << exponent;
                else
                    os << "/2^" << -exponent;
        }
        os << ')';
    }

    template <>
    XReal<real_t>:: operator real_t() const
    { return std::ldexp(mantissa,exponent); }

    template<>
    XReal<real_t>:: XReal(const int e, const real_t m) noexcept :
    exponent(e),
    mantissa(m)
    {
    }


    template <>
    XReal<real_t> XReal<real_t>:: abs() const noexcept
    {
        return XReal( exponent, std::abs(mantissa) );
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

}
