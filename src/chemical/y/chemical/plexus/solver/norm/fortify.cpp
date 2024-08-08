#include "y/chemical/plexus/solver/normalizer.hpp"
#include "y/system/exception.hpp"
#include "y/text/boolean.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        bool Normalizer:: fortify(XWritable &Ctop, const XReadable &Ktop, XMLog &xml)
        {
            Y_XML_SECTION(xml, "Fortify");

            //------------------------------------------------------------------
            //
            //
            // compile all active equilibria
            //
            //
            //------------------------------------------------------------------
            bool         repl = false;
            const size_t napp = compile(Ctop, Ktop, repl, xml);

            if(xml.verbose)
            {
                Y_XML_SECTION(xml,"Landscape");
                for(size_t i=1;i<=napp;++i)
                {
                    const Applicant &app = aps[i];
                    app.display( xml() << "| ", rcl.uuid, false) << " | aff=" << real_t(objectiveFunction(app.cc, SubLevel)) << std::endl;
                }
            }

            switch(napp)
            {
                case 0: Y_XMLOG(xml, "[Inactive] => false"); return false;
                case 1:
                    if(repl)
                    {
                        // one active that replaced Ctop => true
                        Y_XMLOG(xml, "[Replaced] => true");
                        return true;
                    }
                    else
                    {
                        // check if strictly improved affinity
                        const Applicant &app  = aps[1];
                        const XReadable &Csub = app.cc;
                        const xreal_t    aff0 = objectiveFunction(Ctop,TopLevel);
                        const xreal_t    aff1 = objectiveFunction(Csub,SubLevel);
                        const bool       flag = aff1<aff0;
                        Y_XMLOG(xml, "[" << aps[1].eq.name << "] => " << BooleanTo::text(flag) );
                        if(flag)
                        {
                            rcl.transfer(Ctop,TopLevel,Csub,SubLevel);
                        }
                        return flag;
                    }
                default:
                    break;

            }
            assert(napp>=2);






            return false;

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
                    return aff1 < aff0;
                }

                default: // will improve with simplex
                    break;
            }

            const xreal_t aff0 = objectiveFunction(Ctop,TopLevel);
            const xreal_t aff1 = improve(Ctop,repl,xml);
            Y_XMLOG(xml, "affinity: " << real_t(aff0) << " -> " << real_t(aff1) << " @simplex");
            return aff1 < aff0;
        }

    }

}


