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
            if(_nu>1)
            {
                *s += Formatted::Get("%u",_nu);
                switch(_in)
                {
                    case Actor::AsComponentOnly: *s += ' '; break;
                    case Actor::AsConcentration: break;
                }
            }

            
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
        Entity( ActorName(_nu,_sp,how)  ),
        nu( _nu ),
        xn(  nu ),
        sp( _sp ),
        next(0),
        prev(0)
        {
            assert(nu>0);
        }

        Actor:: Actor(const Actor &_) noexcept :
        Entity( _.name ),
        nu(_.nu),
        xn(_.xn),
        sp(_.sp),
        next(0),
        prev(0)
        {
            assert(nu>0);
        }

        void Actor:: activity(XMul &X, const XReadable &C, const Level L) const
        {
            assert(sp(C,L)>=0.0);
            X.insert(sp(C,L),nu);
        }

        void Actor:: activity(XMul &X, const XReadable &C, const Level L, const xreal_t xi) const
        {
            assert(sp(C,L)>=0.0);
            const xreal_t zero;
            X.insert( Max(sp(C,L) + xn * xi,zero), nu);
        }

        xreal_t Actor:: limiting(const XReadable &C, const Level L) const noexcept
        {
            assert(sp(C,L)>=0.0);
            return sp(C,L) / xn;
        }

        void Actor:: moveSafely(XWritable &C, const Level L, const xreal_t xi) const noexcept
        {
            assert(sp(C,L)>=0.0);
            const xreal_t zero;
            sp(C,L) = Max(sp(C,L) + xn * xi,zero);
        }



    }

}
