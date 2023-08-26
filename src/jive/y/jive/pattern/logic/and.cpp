#include "y/jive/pattern/logic/and.hpp"
#include "y/jive/source.hpp"
#include "y/ptr/auto.hpp"

namespace Yttrium
{
    namespace Jive
    {

        And::  And() noexcept : Compound(UUID)
        {
            Y_PATTERN(And);
        }

        And::  And(const And &other) : Compound(other)
        {
            Y_PATTERN(And);
        }

        And:: ~And() noexcept
        {
        }

        Pattern * And:: clone() const
        {
            return new And(*this);
        }

        void And:: query(FirstChars &fc) const
        {
            for(const Pattern *p = patterns.head; p; p=p->next)
            {
                p->query(fc);
                if(p->isRegular())
                    break;
            }
        }

        bool And:: isFragile() const noexcept
        {
            for(const Pattern *p=patterns.head;p;p=p->next)
            {
                if(p->isRegular()) return false;
            }
            return true;
        }

        bool And:: takes(Source &source)
        {
            assert(0==size);

            //------------------------------------------------------------------
            // try to accept all patterns
            //------------------------------------------------------------------
            for(Pattern *p=patterns.head;p;p=p->next)
            {
                assert(0==p->size);
                if(p->takes(source))
                {
                    // one more is accepted
                    mergeTail(*p); assert(0==p->size);
                }
                else
                {
                    // failure here
                    assert(0==p->size);
                    source.put(*this); assert(0==size);
                    return false;
                }
            }

            return true;
        }

        Pattern * Pattern:: Exact(const char *text)
        {
            AutoPtr<Compound> p = new And();
            p->feed(text);
            return p.yield();
        }


        Pattern * Pattern:: Exact(const String &text)
        {
            AutoPtr<Compound> p = new And();
            p->feed(text);
            return p.yield();
        }

        bool And:: isEqualTo(const Pattern &p) const noexcept
        {
            return UUID == p.uuid && hasSamePatternsThan( *p.as<And>() );
        }

        void And:: viz(OutputStream &fp) const
        {
            node(fp) << '[';
            Label(fp, "&&") << ",shape=box";
            fp << ']';
            Endl(fp);
            vizLink(fp);
        }
    }

}

