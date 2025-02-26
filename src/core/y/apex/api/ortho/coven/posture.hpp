
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
                class Posture
                {
                public:
                    explicit Posture(const IBank &bank,
                                     const size_t dims,
                                     const size_t indx) :
                    content(bank,indx),
                    residue(bank,dims,indx)
                    {
                    }

                    explicit Posture(const Posture &     root,
                                     const INode * const node) :
                    content(root.content,**node),
                    residue(root.residue.proxy,node)
                    {
                        assert(root.residue.owns(node));
                        assert(1+root.content->size == content->size);
                        assert(root.residue.size-1  == residue.size);
                    }

                    virtual ~Posture() noexcept {}

                    Y_OSTREAM_PROTO(Posture);


                    void xch(Posture &_) noexcept
                    {
                        content.xch(_.content);
                        residue.xch(_.residue);
                    }

                    bool removed(const size_t indx) noexcept
                    {
                        return content.removed(indx) || residue.removed(indx);
                    }

                    void flush() noexcept
                    {
                        while(residue.size>0)
                            content.push(residue.popHead());
                    }

                    Content content;
                    Residue residue;

                private:
                    Y_DISABLE_ASSIGN(Posture);
                };

               
            }

        }

    }

}

#endif
