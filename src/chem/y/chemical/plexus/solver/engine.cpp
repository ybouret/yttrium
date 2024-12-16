
#include "y/chemical/plexus/solver/engine.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        Engine:: ~Engine() noexcept
        {
        }


        Engine:: Engine() : ProBank(), ProList( static_cast<ProBank &>(*this) ), Assembly() {}


        void Engine:: update() noexcept
        {
            forget();
            for(const ProNode *pn=head;pn;pn=pn->next)
                enroll(**pn);
        }

        void Engine:: reset() noexcept
        {
            forget();
            free();
        }

        void Engine:: show(XMLog &xml) const
        {
            Y_XML_SECTION_OPT(xml, "Chemical::Engine", "count='" << size << "'");
            for(const ProNode *pn=head;pn;pn=pn->next)
            {
                const Prospect &pro = **pn;
                if(xml.verbose) {
                    const char * const info = Components::SituationText(pro.out.st);
                    print(xml() << info << " @",pro,Justify::Left)
                    << " : |xi| = | " << std::setw(15) << real_t(pro.xi) << " |"
                    << " ff = " << std::setw(15) << real_t(pro.ff)
                    << std::endl;
                }
            }
        }

    }

}

