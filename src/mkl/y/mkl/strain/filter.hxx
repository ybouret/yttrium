
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
        }
    }
}
