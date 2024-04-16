//! \file

#ifndef Y_Ink_Format_Options_Included
#define Y_Ink_Format_Options_Included 1

#include "y/string.hpp"

namespace Yttrium
{
    namespace Ink
    {
        
        //______________________________________________________________________
        //
        //
        //
        //!  Database of pasrsed options for Format
        //
        //
        //______________________________________________________________________
        class FormatOptions
        {
        public:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit FormatOptions();             //!< setup empty
            virtual ~FormatOptions() noexcept;    //!< setup emtpy
            FormatOptions(const FormatOptions &); //!< copy

            //! display
            friend std::ostream & operator<<(std::ostream &, const FormatOptions &);

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            FormatOptions & operator<<(const String &opts); //!< append 'opt1=value1:opt2=value2...'
            FormatOptions & operator<<(const char   *opts); //!< wrapper

            const String * query(const String &) const noexcept; //!< query
            const String * query(const char   *) const;          //!< query
            const String * query(const char    ) const;          //!< query

            //! return value/empty
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

