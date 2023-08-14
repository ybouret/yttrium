
#include "y/jive/pattern/logic/or.hpp"
#include "y/jive/source.hpp"
#include "y/ptr/auto.hpp"

namespace Yttrium
{
    namespace Jive
    {

        Or::  Or() noexcept : Compound(UUID) {}

        Or::  Or(const Or &other) : Compound(other) {}

        Or:: ~Or() noexcept
        {
        }

        Pattern * Or:: clone() const
        {
            return new Or(*this);
        }

        void Or:: query(FirstChars &fc) const
        {
            for(const Pattern *p = patterns.head; p; p=p->next)
            {
                p->query(fc);
            }
        }

        bool Or:: isFragile() const noexcept
        {
            for(const Pattern *p=patterns.head;p;p=p->next)
            {
                if(p->isFragile()) return true;
            }
            return false;
        }

        bool Or:: takes(Source &source)
        {
            assert(0==size);

            //------------------------------------------------------------------
            // try to accept one of the patterns
            // if a fragile is accepted, try next but count success
            //------------------------------------------------------------------
            bool success = false;
            for(Pattern *p=patterns.head;p;p=p->next)
            {
                assert(0==p->size);
                if(p->takes(source))
                {
                    success = true;
                    if(p->size>0)
                    {
                        mergeTail(*p); assert(0==p->size);
                        break;
                    }
                    else
                    {
                        assert(p->isFragile());
                        continue; // possible not-empty pattern
                    }
                }

            }

            return success;
        }

        Pattern * Pattern:: Among(const char *text)
        {
            AutoPtr<Compound> p = new Or();
            p->feed(text);
            return p.yield();
        }


        Pattern * Pattern:: Among(const String &text)
        {
            AutoPtr<Compound> p = new Or();
            p->feed(text);
            return p.yield();
        }

        bool Or:: isEqualTo(const Pattern &p) const noexcept
        {
            return UUID == p.uuid && hasSamePatternsThan( *p.as<Or>() );
        }


        void Or:: viz(OutputStream &fp) const
        {
            node(fp) << '[';
            Label(fp, "||") << ",shape=box";
            fp << ']';
            Endl(fp);
            vizLink(fp);
        }

    }

}

