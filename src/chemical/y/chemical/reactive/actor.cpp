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
        nm1(nu-1),
        xn2(nu*nu),
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
        nm1(_.nm1),
        xn2(_.xn2),
        next(0),
        prev(0)
        {
            assert(nu>0);
        }

        void Actor:: activity(XMul &X, const XReadable &C, const Level L) const
        {
            //if(sp(C,L)<0.0) { std::cerr << "Negative [" << sp.name << "]" << std::endl; }
            assert(sp(C,L)>=0.0);
            X.insert(sp(C,L),nu);
        }

        void Actor:: activityError(XMul &X, const XReadable &C, const Level L) const
        {
            assert(sp(C,L)>=0.0);
            (X << xn2).insert(sp(C,L),nm1);
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

        void Actor:: safeMove(XWritable &C, const Level L, const xreal_t xi) const noexcept
        {
            const xreal_t zero;
            assert(sp(C,L)>=zero);
            sp(C,L) = Max(sp(C,L) + xn * xi,zero);
        }


        void Actor:: boldMove(XWritable &C, const Level L, const xreal_t xi) const noexcept
        {
            sp(C,L) += xn*xi;
        }





    }

}
