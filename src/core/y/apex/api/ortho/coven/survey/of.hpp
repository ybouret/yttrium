
//! \file

#ifndef Y_Apex_Ortho_Coven_Survey_Of_Included
#define Y_Apex_Ortho_Coven_Survey_Of_Included 1

#include "y/apex/api/ortho/coven/survey.hpp"
#include "y/apex/api/ortho/coven/survey/sarray.hpp"

namespace Yttrium
{
    namespace Apex
    {
        namespace Ortho
        {
            namespace Coven
            {
                template <typename T>
                class SurveyOf : public Survey, public Proxy< const ListOf< SArray<T> > >
                {
                public:
                    typedef SArray<T>                  ArrayType;
                    typedef Proxy< ListOf<ArrayType> > ProxyType;

                    virtual ~SurveyOf() noexcept
                    {
                    }

                protected:
                    explicit SurveyOf() noexcept : my()
                    {
                    }


                    CxxListOf<ArrayType> my;

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(SurveyOf);
                    inline virtual typename ProxyType::ConstInterface & surrogate() const noexcept
                    {
                        return my;
                    }

                };
            }

        }

    }

}

#endif
