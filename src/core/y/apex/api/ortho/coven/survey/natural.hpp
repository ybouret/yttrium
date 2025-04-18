
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
                //______________________________________________________________
                //
                //
                //
                //! Survey to produce arrays of Natural
                //
                //
                //______________________________________________________________
                class NaturalSurvey : public SurveyOf<Natural>
                {
                public:
                    //__________________________________________________________
                    //
                    //
                    // C++
                    //
                    //__________________________________________________________

                    //! create from data
                    /**
                     keep only vectors of ncof or more positive values
                     \param xml  for output
                     \param data original rows
                     \param ncof minmal number of positive coefficients
                     */
                    template <typename MATRIX> inline
                    explicit NaturalSurvey(XMLog        &   xml,
                                           const MATRIX &   data,
                                           const size_t     ncof,
                                           uint64_t * const pEll) :
                    SurveyOf<Natural>(),
                    minPositiveCount(ncof)
                    {
                        assert(ncof>0);
                        Wayfarer::Explore(xml, proc, data, pEll);
                        finalize();
                    }

                    //! cleanup
                    virtual ~NaturalSurvey() noexcept;

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(NaturalSurvey);
                    const size_t minPositiveCount;
                    virtual void study(const IList &, const Vector &v);

                };

            }
        }

    }

}

#endif

