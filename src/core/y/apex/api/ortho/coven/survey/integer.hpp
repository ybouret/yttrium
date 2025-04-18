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
                     keep only vectors of ncof or more non-zero values
                     \param xml  for output
                     \param data original rows
                     \param ncof minimal number of non-zero coefficients
                     \param pEll optional time counter
                     */
                    template <typename MATRIX> inline
                    explicit IntegerSurvey(XMLog        &   xml,
                                           const MATRIX &   data,
                                           const size_t     ncof,
                                           uint64_t * const pEll) :
                    SurveyOf<Integer>(),
                    minNonZeroCount(ncof)
                    {
                        assert(ncof>0);
                        Wayfarer::Explore(xml, proc, data, pEll);
                        finalize();
                    }

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(IntegerSurvey);
                    virtual void study(const IList &l, const Vector &v);
                    const size_t minNonZeroCount;
                };
            }

        }

    }

}

#endif

