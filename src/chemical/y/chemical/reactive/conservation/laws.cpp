
#include "y/chemical/reactive/conservation/laws.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Conservation
        {

            Laws:: ~Laws() noexcept {}
            Laws:: Laws() noexcept :  Proxy<const Law::List>(), cll() {}

            Laws::ConstInterface & Laws:: surrogate() const noexcept { return cll; }


            void Laws:: writeDown(const Matrix<unsigned> &Qm,
                                  const SpSubSet         &table)

            {
                cll.release();
                for(size_t i=1;i<=Qm.rows;++i)
                {
                    const String label = Formatted::Get("#%lu", static_cast<unsigned long>(i));
                    cll.pushTail( new Law(label,i,Qm[i],table) );
                }
            }

            void Laws:: viz(OutputStream &fp) const
            {
                for(const Law *law=cll.head;law;law=law->next)
                {
                    law->viz(fp);
                }
            }

        }

    }

}


