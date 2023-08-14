#include "y/jive/pattern/logic/none.hpp"
#include "y/jive/source.hpp"
#include "y/jive/pattern/first-chars.hpp"

namespace Yttrium
{
    namespace Jive
    {

        None::  None() noexcept : Compound(UUID) {}

        None::  None(const None &other) : Compound(other) {}

        None:: ~None() noexcept
        {
        }

        Pattern * None:: clone() const
        {
            return new None(*this);
        }

        void None:: query(FirstChars &fc) const
        {
            // assuming all possible chars
            FirstChars result; result.includeAll();

            // remove all possible first chars
            for(const Pattern *p=patterns.head;p;p=p->next)
            {
                FirstChars here;
                p->query(here);
                result.sub(here);
            }

            // combine result
            fc.add(result);
        }

        bool None:: isFragile() const noexcept
        {
            return false;
        }

        bool None:: takes(Source &source)
        {
            assert(0==size);

            //------------------------------------------------------------------
            //
            // if accept a pattern:
            // - is regular, return false
            // - if fragile, skip
            //------------------------------------------------------------------
            for(Pattern *p=patterns.head;p;p=p->next)
            {
                assert(0==p->size);
                if(p->takes(source))
                {
                    if(p->size>0)
                    {
                        source.put(*p); assert(0==p->size);
                        return false;
                    }
                    else
                    {
                        assert(p->isFragile());
                        // doesn't count..
                    }
                }
            }

            // ok, no pattern was found: get next char
            Char *ch = source.get();
            if(ch)
            {
                pushTail(ch);
                return true;
            }
            else
                return false;
        }


        bool None:: isEqualTo(const Pattern &p) const noexcept
        {
            return UUID == p.uuid && hasSamePatternsThan( *p.as<None>() );
        }

        void None:: viz(OutputStream &fp) const
        {
            node(fp) << '[';
            Label(fp, "!=") << ",shape=box";
            fp << ']';
            Endl(fp);
            vizLink(fp);
        }
    }

}

