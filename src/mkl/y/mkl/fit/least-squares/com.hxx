
namespace Yttrium
{

    namespace MKL
    {

        namespace Fit
        {
            template <>
            LeastSquaresCom<real_t>:: LeastSquaresCom() :
            xadd(),
            xlst(),
            beta(),
            curv(),
            bode(3),
            npts(0),
            zero(0),
            half(0.5),
            one(1)
            {
            }

            template <>
            LeastSquaresCom<real_t>:: ~LeastSquaresCom() noexcept
            {
            }

            template <>
            real_t LeastSquaresCom<real_t>:: aabs(const real_t x)
            {
                return Fabs<real_t>::Of(x);
            }

            template <>
            real_t LeastSquaresCom<real_t>:: dot(const Readable<real_t> &lhs, const Readable<real_t> &rhs)
            {
                assert(lhs.size()==rhs.size());
                const size_t n = lhs.size();
                xadd.make(n);
                for(size_t i=n;i>0;--i) xadd << lhs[i] * rhs[i];
                return xadd.sum();
            }

            

        }

    }

}
