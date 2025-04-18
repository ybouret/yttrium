
//! \file

#ifndef Y_Apex_Ortho_Coven_Survey_API_Included
#define Y_Apex_Ortho_Coven_Survey_API_Included 1

#include "y/apex/api/ortho/coven/survey/natural.hpp"
#include "y/apex/api/ortho/coven/survey/integer.hpp"

namespace Yttrium
{
    namespace Apex
    {
        namespace Ortho
        {
            namespace Coven
            {

                namespace API
                {
                    template <typename> struct SurveySelect;

                    template <>
                    struct SurveySelect<Natural> {
                        typedef NaturalSurvey Type;
                    };

                    template <>
                    struct SurveySelect<Integer> {
                        typedef IntegerSurvey Type;
                    };
                }

                template <
                typename     APEX_NUMBER,
                const size_t MINI_COUNT>
                class AutoSurvey : public API::SurveySelect<APEX_NUMBER>::Type
                {
                public:
                    typedef typename API::SurveySelect<APEX_NUMBER>::Type SurveyType;

                    template <typename MATRIX> inline
                    explicit AutoSurvey(XMLog        &   xml,
                                        const MATRIX &   data,
                                        uint64_t * const pEll) :
                    SurveyType(xml,data,MINI_COUNT,pEll)
                    {
                    }

                    inline virtual ~AutoSurvey() noexcept {}
                private:
                    Y_DISABLE_COPY_AND_ASSIGN(AutoSurvey);
                };


            }

        }

    }

}

#endif

