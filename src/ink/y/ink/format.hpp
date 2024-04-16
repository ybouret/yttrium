
//! \file

#ifndef Y_Ink_Format_Included
#define Y_Ink_Format_Included 1

#include "y/ink/bitmap.hpp"
#include "y/ink/format/options.hpp"
#include "y/counted.hpp"

namespace Yttrium
{
    namespace Ink
    {
        class Format : public Object, public Counted
        {
        protected:
            template <typename UID> inline
            explicit Format(const UID &uid) : name(uid) {}

        public:
            virtual ~Format() noexcept;

            virtual bool matches(const String &ext) const noexcept = 0;
            bool         matches(const char   *ext) const noexcept;

            const String &key() const noexcept;

            const String name;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Format);

        };
    }

}

#endif

