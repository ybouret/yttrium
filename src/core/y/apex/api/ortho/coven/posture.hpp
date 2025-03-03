//! \file

#ifndef Y_Apex_Ortho_Coven_Posture_Included
#define Y_Apex_Ortho_Coven_Posture_Included 1

#include "y/apex/api/ortho/coven/content.hpp"
#include "y/apex/api/ortho/coven/residue.hpp"

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
                //! Content+Residue to build a Family
                //
                //
                //______________________________________________________________
                class Posture
                {
                public:
                    //__________________________________________________________
                    //
                    //
                    // C++
                    //
                    //__________________________________________________________

                    //! setup with initial indx for content, other for residue
                    Posture(const IBank &bank,
                            const size_t dims,
                            const size_t indx);

                    //! duplicate and add index to content, excluded from residue
                    Posture(const Posture &     root,
                            const INode * const node);

                    //! duplicate
                    Posture(const Posture &_);

                    //! cleanup
                    virtual ~Posture() noexcept;

                    //! display
                    Y_OSTREAM_PROTO(Posture);

                    //__________________________________________________________
                    //
                    //
                    // Methods
                    //
                    //__________________________________________________________
                    void xch(Posture &) noexcept; //!< no-throw exchange
                    void flush()        noexcept; //!< residue to content

                    //! true if indx was found and removed from content or residue
                    bool removed(const size_t indx) noexcept;

                    //! test equality
                    friend bool operator==(const Posture &,const Posture &) noexcept;

                    //! test difference
                    friend bool operator!=(const Posture &,const Posture &) noexcept;

                    

                    //__________________________________________________________
                    //
                    //
                    // Members
                    //
                    //__________________________________________________________
                    Content content; //!< current content
                    Residue residue; //!< current residue

                private:
                    Y_DISABLE_ASSIGN(Posture);
                };


            }

        }

    }

}

#endif
