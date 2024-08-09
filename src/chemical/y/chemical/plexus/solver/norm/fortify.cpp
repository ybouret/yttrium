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

            switch(napp)
            {
                case 0: Y_XMLOG(xml, "[Inactive] => true"); return false;
                case 1:
                    if(repl)
                    {
                        //------------------------------------------------------
                        //
                        // one active that replaced Ctop => true
                        //
                        //------------------------------------------------------
                        Y_XMLOG(xml, "[Replaced] => true");
                        return true;
                    }
                    else
                    {
                        //------------------------------------------------------
                        //
                        // check if strictly improved affinity
                        //
                        //------------------------------------------------------
                        const Applicant &app  = aps[1];
                        const XReadable &Csub = app.cc;
                        const xreal_t    aff0 = objectiveFunction(Ctop,TopLevel);
                        const xreal_t    aff1 = objectiveFunction(Csub,SubLevel);
                        const bool       flag = aff1<aff0;
                        Y_XMLOG(xml, "[" << app.eq.name << "] => " << BooleanTo::text(flag) );
                        if(flag)
                            rcl.transfer(Ctop,TopLevel,Csub,SubLevel);

                        return flag;
                    }
                default:
                    break;

            }
            assert(napp>=2);


            //------------------------------------------------------------------
            //
            //
            // extract basis from most promising
            //
            //
            //------------------------------------------------------------------
            const size_t dims = extract(xml);
            if(dims<2) throw Specific::Exception(CallSign, "invalid fortified basis");

            return improve(Ctop,repl,xml);

        }


      
    }

}


