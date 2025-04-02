
#include "y/apex/api/ortho/coven/survey/integer.hpp"
#include "y/apex/api/count-non-zero.hpp"

namespace Yttrium
{
    namespace Apex
    {
        namespace Ortho
        {
            namespace Coven
            {

                IntegerSurvey:: ~IntegerSurvey() noexcept {}
                
                void IntegerSurvey:: study(const IList &l, const Vector &v)
                {
                    if(l.size>1 && CountNonZero(v) >= 2)
                    {
                        my.pushTail(new ArrayType(v) );
                        std::cerr << "\t(comb) " << *my.tail << std::endl;
                    }
                }
            }

        }

    }
}

