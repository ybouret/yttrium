
#include "y/lingo/pattern.hpp"
#include <cstdlib>

namespace Yttrium
{
    namespace Lingo
    {
        Pattern:: ~Pattern() noexcept {}

        Pattern:: Pattern(const uint32_t t) noexcept :
        Quantized(), Serializable(), GraphViz::Vizible(),
        uuid(t),
        next(0),
        prev(0),
        self(0)
        {
        }

        Pattern:: Pattern(const Pattern &_) noexcept :
        Quantized(), Serializable(), GraphViz::Vizible(),
        uuid(_.uuid),
        next(0),
        prev(0),
        self(0)
        {
        }

        size_t Pattern:: emitUUID(OutputStream &fp) const
        {
            return fp.emitCBR(uuid);
        }

        bool Pattern:: multiple() const noexcept
        {
            return !univocal();
        }

        bool Pattern:: feeble() const noexcept
        {
            return !strong();
        }




        const char * Pattern:: ByteToRegExp(const uint8_t byte) noexcept
        {
            static const char *rx[256] = {
#               include "pattern/rx.hxx"
            };
            return rx[byte];
        }

        bool operator==(const Pattern &lhs, const Pattern &rhs)
        {
            const String l = lhs.toBinary();
            const String r = rhs.toBinary();
            return l == r;
        }

        Pattern * Pattern:: Among(const char * const text)
        {
            const String _(text);
            return Among(_);
        }

        Pattern * Pattern:: Exact(const char * const text)
        {
            const String _(text);
            return Exact(_);
        }

        void Pattern:: vizLink(OutputStream &fp, const Pattern::List &patterns) const
        {
            for(const Pattern *node=patterns.head;node;node=node->next)
            {
                Endl(Arrow(fp,this,node));
            }
        }

        void Pattern:: graphViz(OutputStream &fp) const
        {
            Enter(fp, "G");
            viz(fp);
            Leave(fp);
        }

    }

}


