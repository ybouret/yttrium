
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

    bool AddressBook:: remove_(const void *addr) noexcept
    {
        const LittleEndianKey key(addr);
        return  AddressMap::remove(key);
    }


    const char * AddressBook:: callSign() const noexcept { return "AddressBook"; }

    void AddressBook:: throwSameAddress(const void *p) const
    {
        throw Specific::Exception( callSign(), "try to insert same address %p",p);
    }


    bool AddressBook:: contains(const AddressBook &sub) const noexcept
    {
        for(ConstIterator it=sub.begin();it!=sub.end();++it)
        {
            if( ! search_(*it) ) return false;
        }
        assert(sub.size()<=size());
        return true;
    }

    bool operator==(const AddressBook &lhs, const AddressBook &rhs) noexcept
    {
        return lhs.size() == rhs.size() && lhs.contains(rhs) && rhs.contains(lhs);
    }



}


