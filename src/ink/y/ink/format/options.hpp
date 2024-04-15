//! \file

#ifndef Y_Ink_Format_Options_Included
#define Y_Ink_Format_Options_Included 1

#include "y/string.hpp"

namespace Yttrium
{
    namespace Ink
    {
        class FormatOptions
        {
        public:
            explicit FormatOptions();
            virtual ~FormatOptions() noexcept;
            FormatOptions(const FormatOptions &);

            friend std::ostream & operator<<(std::ostream &, const FormatOptions &);

            FormatOptions & operator<<(const String &opts);
            FormatOptions & operator<<(const char   *opts);

            const String * query(const String &) const noexcept;
            const String * query(const char   *) const;
            const String * query(const char    ) const;

            template <typename KEY> inline
            const String & operator[](const KEY &key) const
            {
                const String *ps = query(key);
                return ps ? *ps : empty();
            }

        private:
            Y_DISABLE_ASSIGN(FormatOptions);
            const String &empty() const noexcept;
            class Code;
            Code *code;
        };
    }
}

#endif

