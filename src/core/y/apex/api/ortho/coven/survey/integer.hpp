//! \file

#ifndef Y_Apex_Ortho_Coven_Integer_Survey_Included
#define Y_Apex_Ortho_Coven_Integer_Survey_Included 1

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
                //______________________________________________________________
                //
                //
                //
                //! Survey to produce arrays of Integer
                //
                //
                //______________________________________________________________
                class IntegerSurvey : public SurveyOf<Integer>
                {
                public:
                    //__________________________________________________________
                    //
                    //
                    // C++
                    //
                    //__________________________________________________________

                    //! cleanup
                    virtual ~IntegerSurvey() noexcept;


                    //! construct survey from original data
                    /**
                     keep only vectors of two or more non-zero values
                     \param xml  for output
                     \param data original rows
                     \param pEll optional ellapsed ticks
                     */
                    template <typename MATRIX> inline
                    explicit IntegerSurvey(XMLog        &   xml,
                                           const MATRIX &   data,
                                           uint64_t * const pEll)
                    {
                        Wayfarer::Explore(xml, proc, data, pEll);
                        finalize();
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

