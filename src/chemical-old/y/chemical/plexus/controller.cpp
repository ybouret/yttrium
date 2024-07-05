#include "y/chemical/plexus/controller.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        Controller:: Controller(const Equilibrium &eq,
                                const AddressBook &conserved) :
        primary(eq),
        custom(),
        roving(),
        next(0),
        prev(0)
        {
            Components &self = Coerce(custom);
            for(ConstIterator it=eq->begin();it!=eq->end();++it)
            {
                const Component &component = *it;
                const Species   &sp        = component.sp;
                if( conserved.search(sp) )
                {
                    self(component);
                }
                else
                {
                    Coerce(roving) << sp;
                }
            }
            assert(primary->size() == roving.size + custom->size());
        }

        Controller::  ~Controller() noexcept {}


        bool Controller:: isEquivalentTo(const Controller &other) const noexcept
        {
            return custom.isAnalogousTo(other.custom);
        }

        size_t Controller:: operator*() const noexcept
        {
            return primary.indx[AuxLevel];
        }

        
        
    }

}
