
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

         
            Compound & Compound:: operator<<(const Manifest &man)
            {
                manifest << man;
                return *this;
            }


            Compound & Compound:: operator<<(const Rule &rule)
            {
                manifest << rule;
                return *this;
            }

            void Compound:: vizLink(OutputStream &fp) const
            {
                vizArrows(fp,manifest);
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


            void Compound:: noGrammarFor(const Caption &id) const
            {
                throw Specific::Exception(name->c_str(), "no grammar for '%s'", id->c_str());
            }
        }

    }

}


 
