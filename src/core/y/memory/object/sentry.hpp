
//! \file

#ifndef Y_Memory_Object_Sentry_Included
#define Y_Memory_Object_Sentry_Included 1

#include "y/memory/object/guarded.hpp"

namespace Yttrium
{
    namespace Memory
    {
        //______________________________________________________________________
        //
        //
        //! Sentry based on a computed Object Guarded
        //
        //______________________________________________________________________
        class ObjectSentry : public ObjectGuarded, public Sentry
        {
        public:
            explicit ObjectSentry(const ObjectLocator &, ObjectLocator::Query) noexcept; //!< setup
            virtual ~ObjectSentry()                                            noexcept; //!< cleanup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ObjectSentry);
        };
    }

}

#endif
