
namespace Yttrium
{
    namespace MKL
    {
        namespace Strain
        {

            template <> Filter<real_t>:: ~Filter() noexcept
            {
                assert(0!=code);
                Nullify(code);
            }

            template <> Filter<real_t>:: Filter() : code( new Code() ) {}

            template <> const char *  Filter<real_t>:: callSign() const noexcept
            {
                return CallSign;
            }

            template <> size_t Filter<real_t>:: size() const throw()
            {
                return SIZE;
            }

            template <>
            const real_t &  Filter<real_t>:: operator[](const size_t indx) const noexcept
            {
                assert(0!=code);
                return code->coef[indx];
            }

            template <> void Filter<real_t>:: free() noexcept
            {
                assert(0!=code);
                code->points.free();
            }

            template <> void Filter<real_t>:: eval(const real_t x0,
                                                   const size_t degree)
            {
                assert(0!=code);
                code->eval(x0,degree);
            }

            template <> void Filter<real_t>:: add(const real_t x, const real_t y)
            {
                assert(0!=code);
                const V4D<real_t> p(x,y,0,0);
                code->points << p;
            }
        }
    }
}
