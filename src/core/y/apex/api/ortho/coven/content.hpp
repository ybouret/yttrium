//! \file

#ifndef Y_Apex_Ortho_Coven_Content_Included
#define Y_Apex_Ortho_Coven_Content_Included 1

#include "y/apex/api/ortho/coven/types.hpp"
#include "y/type/proxy.hpp"

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
                //! Ordered list of used indices
                //
                //
                //______________________________________________________________
                class Content : public Proxy<const IList>
                {
                public:
                    //__________________________________________________________
                    //
                    //
                    // C++
                    //
                    //__________________________________________________________
                    explicit Content(const IBank &bank) noexcept; //!< setup empty
                    explicit Content(const Content &);            //!< duplicate

                    //! setup with first index
                    explicit Content(const IBank &bank,
                                     const size_t indx);

                    //! copy and append extra indx
                    explicit Content(const Content &root,
                                     const size_t   indx);

                    //! cleanup
                    virtual ~Content() noexcept;

                    //__________________________________________________________
                    //
                    //
                    // Methods
                    //
                    //__________________________________________________________
                    void      xch(Content &)           noexcept; //!< no-throw exchange
                    Content & push(INode * const node) noexcept; //!< push a new node
                    Content & operator<<(const size_t indx);     //!< cleanup

                    //! test equality
                    friend bool operator==(const Content &lhs, const Content &rhs) noexcept;

                    //! test difference
                    friend bool operator!=(const Content &lhs, const Content &rhs) noexcept;


                    //! true if indx was found and removed
                    bool removed(const size_t indx) noexcept;

                private:
                    Y_DISABLE_ASSIGN(Content);
                    Y_PROXY_DECL();
                    IList my;
                    
                    static SignType Compare(const INode * const lhs,
                                            const INode * const rhs) noexcept;
                };
            }
            
        }
        
    }
}

#endif
