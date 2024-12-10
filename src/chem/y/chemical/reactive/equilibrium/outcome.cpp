#include "y/chemical/reactive/equilibrium/outcome.hpp"

namespace Yttrium
{
    namespace Chemical
    {
#if 0
        xReal Outcome:: activity(XMul &xmul, const xReal xi) const
        {
            return eq.activity(xmul, eK, C, L, xi);
        }
#endif

        Outcome:: Outcome(const Situation  &_st,
                                 const Components &_eq,
                                 const xReal       _eK,
                                 const XReadable &_C,
                                 const Level      _L) :
        st(_st),
        eq(_eq),
        eK(_eK),
        C(_C),
        L(_L)
        {

        }

        Outcome:: Outcome(const Outcome &_) noexcept :
        st(_.st),
        eq(_.eq),
        eK(_.eK),
        C(_.C),
        L(_.L)
        {

        }
        
        Outcome:: ~Outcome() noexcept {}


        xReal Outcome:: extent(XAdd &xadd, const XReadable &C0, const Level L0) const
        {
            xadd.free();
            const size_t nc = eq->size(); assert(nc>0);
            {
                size_t n = nc;
                for(Components::ConstIterator it=eq->begin();n>0;--n,++it)
                {
                    const Component &cm    = *it;
                    const Actor     &ac    = cm.actor;
                    const Species   &sp    = ac.sp;
                    const xReal      c_end = sp(C,L);
                    const xReal      c_ini = sp(C0,L0);
                    switch(cm.role)
                    {
                        case Product:  xadd << (c_end - c_ini)/ac.xn; break;
                        case Reactant: xadd << (c_ini - c_end)/ac.xn; break;
                    }
                }
            }
            return xadd.sum() / xReal(nc);
        }

        xReal Outcome:: extent(XAdd &xadd, const XReadable &C0, const Level L0, XWritable &dC, const Level dL) const
        {
            xadd.free();
            const size_t nc = eq->size(); assert(nc>0);
            {
                size_t n = nc;
                for(Components::ConstIterator it=eq->begin();n>0;--n,++it)
                {
                    const Component &cm    = *it;
                    const Actor     &ac    = cm.actor;
                    const Species   &sp    = ac.sp;
                    const xReal      c_end = sp(C,L);
                    const xReal      c_ini = sp(C0,L0);
                    const xReal      delta = sp(dC,dL) = c_end-c_ini;
                    switch(cm.role)
                    {
                        case Product:  xadd <<  delta / ac.xn; break;
                        case Reactant: xadd << -delta / ac.xn; break;
                    }
                }
            }
            return xadd.sum() / xReal(nc);
        }


    }

}

