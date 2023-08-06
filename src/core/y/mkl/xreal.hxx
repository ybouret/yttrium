
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

}
