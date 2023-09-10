//! \file

#ifndef Y_Memory_Object_Sentries_Included
#define Y_Memory_Object_Sentries_Included 1

#include "y/memory/object/sentry.hpp"

namespace Yttrium
{
    namespace Memory
    {
        //______________________________________________________________________
        //
        //
        //! Sentries based on both computed ObjectSentry
        //
        //______________________________________________________________________
        class ObjectSentries : public ObjectLocator
        {
        public:
            explicit ObjectSentries(const void *obj); //!< setup
            virtual ~ObjectSentries()       noexcept; //!< cleanup

            const ObjectSentry prev; //!< sentry at previous memory area
            const ObjectSentry next; //!< sentry at next     memory area

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ObjectSentries);
        };

    }

}

#endif
