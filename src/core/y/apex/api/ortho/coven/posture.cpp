
#include "y/apex/api/ortho/coven/posture.hpp"
namespace Yttrium
{
    namespace Apex
    {
        namespace Ortho
        {
            namespace Coven
            {
                std::ostream & operator<<(std::ostream &os, const Posture &self)
                {
                    os << self.content << ':' << self.residue;
                    return os;
                }


                Posture:: Posture(const IBank &bank,
                        const size_t dims,
                        const size_t indx) :
                content(bank,indx),
                residue(bank,dims,indx)
                {
                }

                Posture:: Posture(const Posture &     root,
                                 const INode * const node) :
                content(root.content,**node),
                residue(root.residue.proxy,node)
                {
                    assert(root.residue.owns(node));
                    assert(1+root.content->size == content->size);
                    assert(root.residue.size-1  == residue.size);
                }

                Posture:: Posture(const Posture &_) :
                content(_.content),
                residue(_.residue)
                {
                }


                Posture:: ~Posture() noexcept {}

                bool operator==(const Posture &lhs, const Posture &rhs) noexcept
                {
                    return (lhs.content == rhs.content) && (lhs.residue == rhs.residue);
                }



                void  Posture:: xch(Posture &_) noexcept
                {
                    content.xch(_.content);
                    residue.xch(_.residue);
                }

                void  Posture:: flush() noexcept
                {
                    while(residue.size>0)
                        content.push(residue.popHead());
                }


                bool  Posture:: removed(const size_t indx) noexcept
                {
                    return content.removed(indx) || residue.removed(indx);
                }

            }

        }

    }
}


