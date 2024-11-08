
#include "y/lingo/syntax/internal/compound.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Syntax
        {
            Compound:: ~Compound() noexcept
            {
            }

            Compound::ConstInterface & Compound:: surrogate() const noexcept { return manifest; }

            void Compound:: add(const Rule &rule) {
                manifest << rule;
            }

            Compound & Compound:: operator<<(const Rule &rule)
            {
                add(rule);
                return *this;
            }

            void Compound:: vizLink(OutputStream &fp) const
            {
                switch(manifest.size)
                {
                    case 0: return;
                    case 1:
                        Endl(Arrow(fp,this,&**manifest.head));
                        return;
                    default:
                        break;
                }

                unsigned i = 1;
                for(const RNode *node = manifest.head;node;node=node->next,++i)
                {
                    Arrow(fp,this,&**manifest.head) << '[';
                    const String txt = Formatted::Get("%u",i);
                    Label(fp,txt);
                    Endl(fp<<']');
                }

            }


            void Compound:: checkNotEmpty(const char * const fn,
                                          const char * const id) const
            {
                assert(0!=fn);
                assert(0!=id);
                if(manifest.size<=0)
                    throw Specific::Exception(fn,"empty '%s'",id);
            }

            String Compound:: MakeNameFor(const Manifest &list,
                                          const char      sep)
            {
                switch(list.size)
                {
                    case 0: return String();
                    case 1: return *(**(list.head)).name;
                    default:
                        break;
                }
                String ans  = '(';
                {
                    const RNode *node = list.head;
                    ans += *(**node).name;
                    for(node=node->next;node;node=node->next)
                    {
                        ans += sep;
                        ans += *(**node).name;
                    }
                }
                return ans += ')';
            }

        }

    }

}


