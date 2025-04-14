#include "y/apex/api/ortho/coven/survey/natural.hpp"

namespace Yttrium
{
    namespace Apex
    {
        namespace Ortho
        {
            namespace Coven
            {
                void NaturalSurvey:: study(const IList &, const Vector &v)
                {
                    size_t numPositive = 0;
                    for(size_t i=v.size();i>0;--i)
                    {
                        switch( v[i].s )
                        {
                            case Negative: return;
                            case __Zero__: continue;
                            case Positive:
                                ++numPositive;
                                continue;
                        }
                    }
                    if(numPositive>=2)
                    {
                        my.pushTail(new ArrayType(v) );
                        //std::cerr << "\t(keep) " << *my.tail << std::endl;
                    }
                }


                NaturalSurvey:: ~NaturalSurvey() noexcept {}
            }

        }

    }

}

