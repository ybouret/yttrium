
#include "y/associative/address-book.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    AddressBook:: AddressBook() : AddressMap()
    {
    }

    AddressBook:: ~AddressBook() noexcept
    {
    }

    AddressBook:: AddressBook(const AddressBook &other) :
    Identifiable(), Collection(),
    AddressMap(other)
    {
    }


    bool AddressBook:: insert_(const void *addr)
    {
        const LittleEndianKey key(addr);
        return  AddressMap::insert(key,(void*)addr);
    }

    bool AddressBook:: search_(const void *addr) const noexcept
    {
        const LittleEndianKey key(addr);
        return  AddressMap::search(key);
    }

    const char * AddressBook:: callSign() const noexcept { return "AddressBook"; }

    void AddressBook:: throwSameAddress(const void *p) const
    {
        throw Specific::Exception( callSign(), "try to insert same address %p",p);
    }

}


