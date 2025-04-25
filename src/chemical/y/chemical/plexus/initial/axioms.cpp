
#include "y/chemical/plexus/initial/axioms.hpp"
#include "y/chemical/plexus/initial/design.hpp"

namespace Yttrium
{
    namespace Chemical
    {
        namespace Initial
        {

            static inline
            const String * extractLogoFrom(const XCode &code)
            {
                assert(code->defines<Design>());
                const XList        & list = code->branch(); assert(list.size>0);
                const XNode * const  logo = list.head;      assert(Design::Logo==logo->name());
                const String         uuid = logo->lexeme().toString();
                return new String(uuid);
            }

            Axioms:: Axioms(const XCode &code) :
            Entity( extractLogoFrom(code) ),
            xcode(code)
            {
                delete Coerce(xcode->branch()).popHead();
            }



            Axioms:: Axioms(const Axioms &_) noexcept :
            Entity(_.name),
            xcode(_.xcode)
            {
            }

            Axioms:: ~Axioms() noexcept
            {
            }

            Y_PROXY_IMPL(Axioms,xcode->branch())
            

            std::ostream & operator<<(std::ostream &os, const Axioms &axioms)
            {

                os << axioms.name << ':';
                for(const XNode * node=axioms->head;node;node=node->next)
                {
                    os << ' ';
                    switch(node->type)
                    {
                        case XNode::Terminal: os << node->lexeme(); break;
                        case XNode::Internal: os << node->name();   break;
                    }

                }
                return os;
            }
        }

    }

}


