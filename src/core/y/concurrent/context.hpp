
//! \file

#ifndef Y_Concurrent_Context_Included
#define Y_Concurrent_Context_Included 1

#include "y/memory/buffer/ro.hpp"

namespace Yttrium
{

    namespace Concurrent
    {
        //______________________________________________________________________
        //
        //
        //
        //! possible properties of a context
        //
        //
        //______________________________________________________________________
        enum Property
        {
            OnlyOne, //!< sequential
            Leading, //!< parallel, rank=0
            Generic, //!< parallel, rank>0, rank<size-1
            Closing  //!< parallel, rank==size-1
        };

        //______________________________________________________________________
        //
        //
        //
        //! Generic parallel context : size, rank, indx, name
        //
        //
        //______________________________________________________________________
        class Context : public Memory::ReadOnlyBuffer
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Context(const size_t sz, const size_t rk) noexcept; //!< size.rank
            explicit Context()                                 noexcept; //!< 1.0
            virtual ~Context()                                 noexcept; //!< cleanup
            Context(const Context &)                           noexcept; //!< copy

            friend std::ostream & operator<<(std::ostream &, const Context &);
            
            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________
            virtual const void * ro_addr() const noexcept; //!< name
            virtual size_t       measure() const noexcept; //!< len_

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            size_t next1D() const noexcept; //!< next in 1D
            size_t prev1D() const noexcept; //!< prev in 1D
            


            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const size_t   size;     //!< size
            const size_t   rank;     //!< rank
            const size_t   indx;     //!< rank+1
            const size_t   last;     //!< size-1
            const Property ppty;     //!< rank w.r.t size
            const bool     parallel; //!< size>1
            const bool     primary;  //!< rank==0
            const bool     replica;  //!< rank>0
            const char     name[48]; //!< size.rank, aligned
            const size_t   len_;     //!< strlen(name)

        private:
            Y_DISABLE_ASSIGN(Context);

        };
    }

}

#endif
