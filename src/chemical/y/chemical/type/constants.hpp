
//! \file

#ifndef Y_Chemical_Types_Constants_Included
#define Y_Chemical_Types_Constants_Included 1

#include "y/chemical/type/defs.hpp"
#include "y/sequence/vector.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/counted.hpp"
#include "y/ptr/arc.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //! base type to store constants
        //
        //______________________________________________________________________
        typedef Vector<xreal_t,MemoryModel> XVectorType;
        
        //______________________________________________________________________
        //
        //
        //
        //! making a reference-counted vector
        //
        //
        //______________________________________________________________________
        class XVector : public Object, public Counted, public XVectorType
        {
        public:
            explicit XVector() noexcept; //!< create
            virtual ~XVector() noexcept; //!< cleanup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(XVector);
        };

        //______________________________________________________________________
        //
        //
        //
        //! sharing constants among data
        //
        //
        //______________________________________________________________________
        class Constants : public ArcPtr<XVector>
        {
        public:
            Constants();                           //!< create a new shared XVector
            Constants(const Constants &) noexcept; //!< no-throw shared copy
            virtual ~Constants()         noexcept; //!< cleanup

        private:
            Y_DISABLE_ASSIGN(Constants);
        };


    }
}

#endif
