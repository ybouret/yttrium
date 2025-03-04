//! \file

#ifndef Y_Apex_Ortho_Coven_Survey_Included
#define Y_Apex_Ortho_Coven_Survey_Included 1

#include "y/apex/api/ortho/coven/tribes.hpp"

namespace Yttrium
{
    namespace Apex
    {
        namespace Ortho
        {
            namespace Coven
            {
                class Survey
                {
                protected:
                    virtual ~Survey() noexcept
                    {
                    }

                public:
                    explicit Survey() :
                    proc(this, & Survey::check)
                    {
                    }


                    virtual void study(const IList &l, const Vector &v) = 0;



                protected:
                    Callback proc;

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Survey);

                    void check(const IList &l, const Vector &v)
                    {
                        study(l,v);
                    }
                };
            }
        }
    }
}

#endif

