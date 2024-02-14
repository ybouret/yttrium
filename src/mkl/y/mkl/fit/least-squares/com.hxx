
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
            
        }

    }

}
