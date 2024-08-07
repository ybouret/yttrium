#include "y/chemical/plexus/solver/normalizer.hpp"

#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        bool Normalizer:: fortify(XWritable &Ctop, const XReadable &Ktop, XMLog &xml)
        {
            Y_XML_SECTION(xml, "Fortify");

            bool         repl = false;
            const size_t napp = compile(Ctop, Ktop, repl, xml);
            if(napp<=0)
            {
                Y_XMLOG(xml, "[Jammed!]");
                return true;
            }

            const size_t dims = extract(xml);
            switch(dims)
            {
                case 0:   // shouldn't happen
                    throw Specific::Exception(CallSign, "couldn't find fortified basis");

                case 1:  // use single winner
                {
                    const Applicant &app  = **apl.head;
                    const xreal_t    aff0 = objectiveFunction(Ctop,TopLevel);
                    const xreal_t    aff1 = objectiveFunction(rcl.transfer(Ctop,TopLevel,app.cc,SubLevel),TopLevel);
                    Y_XMLOG(xml, "affinity: " << real_t(aff0) << " -> " << real_t(aff1) << " @" << app.eq.name);
                    return aff1 <= aff0;
                }

                default: // will improve with simplex
                    break;
            }

            const xreal_t aff0 = objectiveFunction(Ctop,TopLevel);
            const xreal_t aff1 = improve(Ctop,repl,xml);
            Y_XMLOG(xml, "affinity: " << real_t(aff0) << " -> " << real_t(aff1) << " @simplex");
            return aff1 <= aff0;
        }

    }

}


