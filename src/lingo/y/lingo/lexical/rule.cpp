
#include "y/lingo/lexical/rule.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Lexical
        {
            Rule:: ~Rule() noexcept
            {
                assert(0!=motif);
                delete motif;
                Coerce(motif) = 0;
            }

            Rule:: Rule(const Caption &  _rname,
                        Pattern * const  _motif,
                        const Callback & _xcode) noexcept :
            Entity(_rname,AsCaption),
            motif(_motif),
            xcode(_xcode),
            next(0),
            prev(0)
            {

            }

            void Rule:: ErrorFeeblePattern(const String &rid)
            {
                throw Specific::Exception("Lexical::Rule", "feeble pattern for '%s'", rid.c_str());
            }

            Rule * Rule:: Create(const Caption    & rname,
                                 AutoPtr<Pattern> & motif,
                                 const Callback   & xcode)
            {
                if(motif->feeble()) ErrorFeeblePattern(*rname);
                Rule * const rule = new Rule(rname,& *motif, xcode);
                motif.relax();
                return rule;
            }

        }

    }

}
