
#include "y/chemical/plexus/solver/squad.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Squad:: ~Squad() noexcept {}

        Squad:: Squad(const PBank &probank) noexcept :
        Quantized(),
        PList(probank),
        next(0),
        prev(0)
        {
        }
        

        bool Squad:: accept(const Prospect     &pro,
                            const Matrix<bool> &attached) noexcept
        {
            assert(!has(pro));
            const Readable<bool> &flag = attached[pro.indx()];
            for(const PNode *pn=head;pn;pn=pn->next)
            {
                if( flag[ (**pn).indx() ]) return true;
            }
            return false;
        }

    }

}
