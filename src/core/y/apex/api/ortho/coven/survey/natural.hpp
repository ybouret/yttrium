
//! \file

#ifndef Y_Apex_Ortho_Coven_Natural_Survey_Included
#define Y_Apex_Ortho_Coven_Natural_Survey_Included 1

#include "y/apex/api/ortho/coven/survey/of.hpp"
#include "y/apex/api/ortho/coven/survey/wayfarer.hpp"

namespace Yttrium
{
    namespace Apex
    {
        namespace Ortho
        {
            namespace Coven
            {
                class NaturalSurvey : public SurveyOf<Natural>
                {
                public:
                    explicit NaturalSurvey() noexcept {}
                    virtual ~NaturalSurvey() noexcept {}

                    template <typename MATRIX> inline
                    explicit NaturalSurvey(XMLog        &   xml,
                                           const MATRIX &   data,
                                           uint64_t * const pEll)
                    {
                        Wayfarer::Explore(xml, proc, data, pEll);
                    }

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(NaturalSurvey);

                    virtual void study(const IList &, const Vector &v);

                };

            }
        }

    }

}

#endif

