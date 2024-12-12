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

        Fragment:: Fragment(const Fragment &_) noexcept :
        Proxy<const Assembly>(),
        self(_.self),
        reac(_.reac),
        prod(_.prod)
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

        void Fragment:: trades(Fragment &_) noexcept
        {
            self.tradeFor(_.self);
            reac.tradeFor(_.reac);
            prod.tradeFor(_.prod);
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

        std::ostream & Fragment:: print(std::ostream &os, const Components &cm, const xReal K) const
        {
            const real_t l10 = K.log10();
            print(os,cm) << "'10^(" << l10 << ")'";
            return os;
        }

    }

}
