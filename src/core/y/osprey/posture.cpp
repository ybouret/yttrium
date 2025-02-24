
#include "y/osprey/posture.hpp"

namespace Yttrium
{
    namespace Osprey
    {
        Posture:: Posture(const IBank &bank,
                         const size_t dims,
                         const size_t excl) :
        content(bank,excl),
        residue(bank,dims,excl)
        {
        }

        Posture:: Posture(const Posture &     root,
                          const INode * const node) :
        content(root.content,**node),
        residue(root.residue->proxy,node)
        {
            assert(root.residue->owns(node));
            assert(root.residue->size-1==residue->size);
            assert(root.content->size+1==content->size);
        }

        Posture:: Posture(const Posture &root) :
        content(root.content),
        residue(root.residue)
        {

        }

        Posture:: ~Posture() noexcept
        {

        }

        bool Posture:: AreEqual(const Posture &lhs, const Posture &rhs) noexcept
        {
            return IList::AreEqual(*lhs.content,*rhs.content) && IList::AreEqual(*lhs.residue,*rhs.residue);
        }


        bool Posture:: tryRemove(const size_t indx) noexcept
        {
            return content.tryRemove(indx) || residue.tryRemove(indx);
        }

        void Posture:: flush() noexcept
        {
            content << residue;
        }

        std::ostream & operator<<(std::ostream &os, const Posture &self)
        {
            os << self.content << ':' << self.residue;
            return os;
        }
    }

}


