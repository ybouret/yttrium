
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

                    //! select NaturalSurvey
                    template <>
                    struct SurveySelect<Natural> {
                        typedef NaturalSurvey Type; //!< alias
                    };

                    //! select Integer Survey
                    template <>
                    struct SurveySelect<Integer> {
                        typedef IntegerSurvey Type; //!< alias
                    };
                }

                //______________________________________________________________
                //
                //
                //
                //! select apz/apz survey with minimal number of coefficients
                //
                //
                //______________________________________________________________
                template <
                typename     APEX_NUMBER,
                const size_t MIN_COUNT>
                class AutoSurvey : public API::SurveySelect<APEX_NUMBER>::Type
                {
                public:
                    //! alias
                    typedef typename API::SurveySelect<APEX_NUMBER>::Type SurveyType;

                    //! create survey
                    template <typename MATRIX> inline
                    explicit AutoSurvey(XMLog        &   xml,
                                        const MATRIX &   data,
                                        uint64_t * const pEll) :
                    SurveyType(xml,data,MIN_COUNT,pEll)
                    {
                    }

                    //! cleanup
                    inline virtual ~AutoSurvey() noexcept {}
                private:
                    Y_DISABLE_COPY_AND_ASSIGN(AutoSurvey);
                };


            }

        }

    }

}

#endif

