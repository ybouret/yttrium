
#include "y/chemical/plexus/cluster/conservation/laws.hpp"



namespace Yttrium
{
    namespace Chemical
    {
        namespace Conservation
        {

            Laws:: ~Laws() noexcept
            {
            }

            Laws:: Laws(const SList            &sp,
                        const Matrix<unsigned> &Qm) :
            laws()
            {
                assert(Qm.cols==sp.size);
                for(size_t i=1;i<=Qm.rows;++i)
                {
                    laws.pushTail( new Law(sp,Qm[i]) );
                }
            }

            Laws:: ConstInterface & Laws:: surrogate() const noexcept
            {
                return laws;
            }

        }

    }
}
