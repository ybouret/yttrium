//! \file

#ifndef Y_Apex_Ortho_Coven_Integer_Survey_Included
#define Y_Apex_Ortho_Coven_Integer_Survey_Included 1

#include "y/apex/api/ortho/coven/survey-of.hpp"
#include "y/apex/api/ortho/coven/wayfarer.hpp"

namespace Yttrium
{
    namespace Apex
    {
        namespace Ortho
        {
            namespace Coven
            {
                
                class IntegerSurvey : public SurveyOf<Integer>
                {
                public:
                    explicit IntegerSurvey() noexcept {}
                    virtual ~IntegerSurvey() noexcept {}

                    template <typename MATRIX> inline
                    explicit IntegerSurvey(XMLog        &   xml,
                                           const MATRIX &   data,
                                           uint64_t * const pEll)
                    {
                        Wayfarer::Explore(xml, proc, data, pEll);
                    }

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(IntegerSurvey);
                    virtual void study(const IList &l, const Vector &v);
                };
            }

        }

    }

}

#endif

