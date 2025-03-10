#include "y/chemical/reactive/actor.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        const char * const Actor:: CallSign = "Actor";


        Actor:: ~Actor() noexcept
        {
        }



        static inline
        const String * ActorName(const unsigned     _nu,
                                 const Species  &   _sp,
                                 const Actor::Involvement _in)
        {

            if(_nu<=0) throw Specific::Exception(Actor::CallSign,"zero '%s'", _sp.name->c_str());
            AutoPtr<String> s = new String();
            if(_nu>1) *s += Formatted::Get("%u ",_nu);
            switch(_in)
            {
                case Actor::AsComponentOnly: *s += * _sp.name; break;
                case Actor::AsConcentration: *s += '[' + * _sp.name + ']'; break;
            }
            return s.yield();
        }

        Actor:: Actor(const unsigned   _nu,
                      const Species  & _sp,
                      const Involvement how):
        Entity( ActorName(_nu,_sp,how) ),
        nu( _nu ),
        sp( _sp ),
        in( how ),
        next(0),
        prev(0)
        {

        }


    }

}
