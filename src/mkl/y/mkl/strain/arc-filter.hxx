namespace Yttrium
{
    namespace MKL
    {
        namespace Strain
        {

            template <>
            ArcFilter<real_t>:: ~ArcFilter() noexcept
            {
                assert(0!=code);
                Nullify(code);
            }

            template <>
            ArcFilter<real_t>:: ArcFilter() :
            code( new Code() )
            {
                
            }


            template <>
            void ArcFilter<real_t>:: free() noexcept
            {
                assert(0!=code);
                code->free();
            }
        }
    }
}
