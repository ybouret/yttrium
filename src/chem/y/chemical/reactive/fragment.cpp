#include "y/chemical/reactive/fragment.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Fragment:: Fragment() noexcept :
        Proxy<const Assembly>(),
        self(),
        reac(),
        prod()
        {
        }

        Fragment:: ~Fragment() noexcept
        {
        }

        Fragment::ConstInterface & Fragment:: surrogate() const noexcept
        {
            return self;
        }


        void Fragment:: enroll(const Components &cm)
        {
            self.enroll(cm);
            reac.enroll(*cm.reac);
            prod.enroll(*cm.prod);
        }

        void Fragment:: forget() noexcept
        {
            self.forget();
            reac.forget();
            prod.forget();
        }




    }

}

#include "y/chemical/weasel.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        std::ostream & Fragment:: print(std::ostream &os, const Components &cm) const
        {
            self.print(os, cm, Justify::Left);
            os << Weasel::COLON << ' ';
            reac.print(os, *cm.reac, Justify::Right);
            os << ' ' << Weasel::SYMBOL << ' ';
            prod.print(os, *cm.prod, Justify::Left);
            os << ' ' << Weasel::COLON;
            return os;
        }
    }

}
