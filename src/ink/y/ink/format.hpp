
//! \file

#ifndef Y_Ink_Format_Included
#define Y_Ink_Format_Included 1

#include "y/ink/bitmap.hpp"
#include "y/ink/format/options.hpp"
#include "y/counted.hpp"
#include "y/ptr/ark.hpp"

namespace Yttrium
{
    namespace Ink
    {
        class Format : public Object, public Counted
        {
        public:
            typedef ArkPtr<String,Format> Handle;

        protected:
            template <typename UID> inline
            explicit Format(const UID &uid) : name(uid) {}

        public:
            virtual ~Format() noexcept;

            bool matches(const String &      ) const noexcept;
            bool matches(const char   * const) const noexcept;


            const String &key() const noexcept;

            const String  name;

        protected:
            static bool CaseInsensitiveMatch(const char * const lhs, const size_t lsz,
                                             const char * const rhs, const size_t rsz) noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Format);
            virtual bool matches(const char * const ext, const size_t len) const noexcept = 0;
        };
    }

}

#endif

