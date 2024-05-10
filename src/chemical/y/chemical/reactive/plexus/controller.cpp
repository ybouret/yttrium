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

    }

}
