
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
            Law::List(), Assembly(), groups()
            {
                //--------------------------------------------------------------
                //
                // create one law per conservation row
                //
                //--------------------------------------------------------------
                assert(Qm.cols==sp.size);
                for(size_t i=1;i<=Qm.rows;++i)
                    enroll( *pushTail( new Law(sp,Qm[i]) ) );


                //--------------------------------------------------------------
                //
                // create groups
                //
                //--------------------------------------------------------------

            }

        }

    }
}

namespace Yttrium
{
    namespace Chemical
    {
        namespace Conservation
        {
            Laws:: Group:: ~Group() noexcept
            {
            }

            Laws:: Group:: Group(const Law &first) :
            Object(),
            LList(),
            next(0),
            prev(0)
            {
                (*this) << first;
            }
        }
    }

}
