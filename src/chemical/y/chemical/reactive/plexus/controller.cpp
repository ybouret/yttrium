#include "y/chemical/reactive/plexus/controller.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Controller:: Controller(const Equilibrium &eq,
                                const AddressBook &conserved) :
        primary(eq),
        components(),
        next(0),
        prev(0)
        {
            Components &self = Coerce(components);
            for(ConstIterator it=eq->begin();it!=eq->end();++it)
            {
                const Component &component = *it;
                if( conserved.search(component.sp) ) self(component);
            }
        }

        Controller::  ~Controller() noexcept {}


        bool Controller:: isEquivalentTo(const Controller &other) const noexcept
        {
            return components.isAnalogousTo(other.components);
        }

        void Controller:: shape(Landscape       &landscape,
                                const XReadable &C,
                                const Level      level) const
        {
            const xreal_t zero = 0;
            landscape.reset();

            // scan reactants
            for(const Actor *a=components.reac.head;a;a=a->next)
            {
                const Species &sp = a->sp;
                const xreal_t  nu = a->xnu;
                const xreal_t  cc = C[sp.indx[level]];
                if(cc>=zero)
                {
                    landscape.capping.reac(sp,cc/nu);
                }
                else
                {
                    landscape.missing.reac(sp,(-cc)/nu);
                }
            }

            // scan products
            for(const Actor *a=components.prod.head;a;a=a->next)
            {
                const Species &sp = a->sp;
                const xreal_t  nu = a->xnu;
                const xreal_t  cc = C[sp.indx[level]];
                if(cc>=zero)
                {
                    landscape.capping.prod(sp,cc/nu);
                }
                else
                {
                    landscape.missing.prod(sp,(-cc)/nu);
                }
            }
        }

    }

}
