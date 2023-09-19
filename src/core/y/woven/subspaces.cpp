
#include "y/woven/subspaces.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace WOVEn
    {

        const char * const SubSpaces:: CallSign = "WOVEn::SubSpaces";

        const char * SubSpaces:: callSign() const noexcept { return CallSign; }

        SubSpaces :: ~SubSpaces() noexcept
        {
        }

        std::ostream & operator<<(std::ostream &os, const SubSpaces &subs)
        {
            const XMLog &xml = subs.xml;
            xml.indent(os) << '{' << std::endl;
            for(const SubSpace *s=subs.head;s;s=s->next)
            {
                xml.indent(os) << "  " << *s << std::endl;
            }
            xml.indent(os) << '}';
            return os;
        }


     

        void SubSpaces:: pack()
        {
            SubSpace::List temp;
            while(size>0)
            {
                AutoPtr<SubSpace> source = popHead();

                if(generation!=source->size)
                    throw Specific::Exception(CallSign,"Invalid SubSpace#%u @generation=%u", unsigned(source->size), unsigned(generation) );

                for(SubSpace *target=temp.head;target;target=target->next)
                {
                    if(target->merged(source))
                    {
                        assert(source.isEmpty());
                        goto CYCLE;
                    }
                }
                assert(source.isValid());
                temp.pushTail( source.yield() );
            CYCLE:
                continue;
            }
            swapWith(temp);

        }


        void SubSpaces:: conductInitial(QSurvey &survey) const
        {
            Y_XMLOG(xml, "-- initial survey...");
            for(const SubSpace *sub = head; sub; sub=sub->next)
            {
                assert(sub->size>0);
                survey(*(sub->tail));
            }
        }

    }

}
