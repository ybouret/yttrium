
//! \file

#ifndef Y_Address_Book_Included
#define Y_Address_Book_Included 1

#include "y/associative/suffix/map.hpp"
#include "y/associative/little-endian-address.hpp"
#include "y/ptr/core.hpp"

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
        bool remove_(const void *addr)       noexcept; //!< remove address

        //! search an object's address
        template <typename T> inline bool search(T &obj) const noexcept { return search_( &obj ); }

        //! insert a new object's address
        template <typename T> inline bool insert(T &obj) { return insert_( &obj ); }

        //! remove an object's address
        //template <typename T> inline bool remove(T &obj) noexcept { return remove_( &obj ); }


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

        //! look for object address
        template <typename T> inline
        bool has(const T &obj) const noexcept { return search_( &obj ); }


        //! display as a set of T
        template <typename T> inline
        std::ostream & display(std::ostream &os) const
        {
            os << '{';
            size_t n = size();
            if(n>0)
            {
                ConstIterator it = begin();
                Display<T>(os,it);
                for(--n,++it;n>0;--n,++it)
                    Display<T>(os<<',',it);
            }
            os << '}';
            return os;
        }

        //! helper with conversion
        template <typename TARGET> inline
        TARGET & sendTo(TARGET &target) const
        {
            for(ConstIterator it=begin();it!=end();++it)
                target << *static_cast<typename TARGET::ConstType *>( *it );
            return target;
        }

        //! check if all addresses in sub are here
        bool contains(const AddressBook &sub) const noexcept;

        //! same size and contains each other
        friend bool operator==(const AddressBook &lhs, const AddressBook &rhs) noexcept;




    private:
        Y_DISABLE_ASSIGN(AddressBook);
        void throwSameAddress(const void *p) const;
        
        template <typename T> static inline
        void Display(std::ostream &os, const ConstIterator &it)
        {
            const void * const addr = *it;
            if(0==addr) os << Core::Ptr::Nil; else os << *static_cast<const T *>(addr);
        }

    };

}

#endif

