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
                //______________________________________________________________
                //
                //
                //
                //! Survey interface to explore data
                //
                //
                //______________________________________________________________
                class Survey
                {
                protected:
                    //__________________________________________________________
                    //
                    //
                    // C++
                    //
                    //__________________________________________________________
                    virtual ~Survey() noexcept; //!< cleanup

                public:
                    explicit Survey(); //!< setup proc

                    //__________________________________________________________
                    //
                    //
                    // Interface
                    //
                    //__________________________________________________________

                    //! what to do with vector created from l=content of posture
                    virtual void study(const IList &l, const Vector &v) = 0;

                    //__________________________________________________________
                    //
                    //
                    // Members
                    //
                    //__________________________________________________________
                protected:
                    Callback proc; //!< redirect to study

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Survey);
                    void check(const IList &l, const Vector &v);
                };
            }
        }
    }
}

#endif

