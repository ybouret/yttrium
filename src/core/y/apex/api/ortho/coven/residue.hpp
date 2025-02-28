
//! \file

#ifndef Y_Apex_Ortho_Coven_Residue_Included
#define Y_Apex_Ortho_Coven_Residue_Included 1

#include "y/apex/api/ortho/coven/types.hpp"

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
                //! List of available indices
                //
                //
                //______________________________________________________________
                class Residue : public IList
                {
                public:
                    //__________________________________________________________
                    //
                    //
                    // C++
                    //
                    //__________________________________________________________
                    explicit Residue(const IBank &bank) noexcept; //!< setup empty
                    explicit Residue(const Residue &);            //!< duplicate

                    //! setup with [1..dims] without excl
                    explicit Residue(const IBank &bank,
                                     const size_t dims,
                                     const size_t excl);


                    //! setup from content of node's list, without node
                    explicit Residue(const IBank &       bank,
                                     const INode * const node);

                    //! cleanup
                    virtual ~Residue() noexcept;

                    //__________________________________________________________
                    //
                    //
                    // Methods
                    //
                    //__________________________________________________________

                    //! no-throw exchange
                    void xch(Residue &_) noexcept;

                    //! true if indx was found and removed
                    bool removed(const size_t indx) noexcept;

                    //! test equality
                    friend bool operator==(const Residue &, const Residue &) noexcept;

                    //! test difference
                    friend bool operator!=(const Residue &, const Residue &) noexcept;


                private:
                    Y_DISABLE_ASSIGN(Residue);
                };

            }

        }

    }

}

#endif

