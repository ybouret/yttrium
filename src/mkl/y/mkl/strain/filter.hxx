
namespace Yttrium
{
    namespace MKL
    {
        namespace Strain
        {

            template <> Filter<real_t>:: ~Filter() noexcept
            {
                if(0!=code) Nullify(code);
            }

            template <> Filter<real_t>:: Filter() noexcept : code(0) {}
        }
    }
}
