//! \file

#ifndef Y_Field_IDB_Included
#define Y_Field_IDB_Included 1

#include "y/singleton.hpp"
#include "y/string.hpp"
#include "y/ptr/arc.hpp"

namespace Yttrium
{
    namespace Field
    {

        typedef ArcPtr<String> SharedString;

        class IndexDataBase : public Singleton<IndexDataBase>
        {
        public:
            static const char * const      CallSign;
            static const AtExit::Longevity LifeTime = AtExit::MaximumLongevity - 15;
            static const char              Prefix = '[';
            static const char              Suffix = ']';

            const String &operator[](const unit_t i);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(IndexDataBase);
            explicit IndexDataBase();
            virtual ~IndexDataBase() noexcept;
            friend class Singleton<IndexDataBase>;

        };


    }
}

#endif

