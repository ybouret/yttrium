
//! \file

#ifndef Y_Address_Book_Included
#define Y_Address_Book_Included 1

#include "y/associative/suffix/map.hpp"
#include "y/associative/little-endian-address.hpp"

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //! base class for address book
    //
    //__________________________________________________________________________
    typedef  SuffixMap<LittleEndianKey,void *> AddressMap;

    //__________________________________________________________________________
    //
    //
    //
    //! Storing addresses in compact manner
    //
    //
    //__________________________________________________________________________
    class AddressBook : public AddressMap
    {
    public:
        using AddressMap::search; // to avoid overloaded virtual
        using AddressMap::insert; // to avoid overloaded virtual

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
        explicit AddressBook();           //!< setup
        virtual ~AddressBook() noexcept;  //!< cleanup
        AddressBook(const AddressBook &); //!< copy

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        virtual const char * callSign() const throw(); //!< "AddressBook"
        bool insert_(const void *addr);                //!< insert new address
        bool search_(const void *addr) const noexcept; //!< search address


        //! search an object's address
        template <typename T> inline bool search(T &obj) const { return search_( &obj ); }

        //! insert a new object's address
        template <typename T> inline bool insert(T &obj) { return insert_( &obj ); }

        //! append a NEW object's address
        template <typename T> inline
        AddressBook & operator+=( T &obj )
        {
            if(!insert(obj)) throwSameAddress( &obj );
            return *this;
        }

        //! record any object's address
        template <typename T> inline
        AddressBook & operator|=( T &obj ) { (void) insert(obj); return *this; }

    private:
        Y_DISABLE_ASSIGN(AddressBook);
        void throwSameAddress(const void *p) const;

    };

}

#endif

