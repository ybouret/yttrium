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
            String       & fetch(const String &key);             //!< create new empty / recall
            String       & fetch(const char *);                  //!< wrapper
            String       & fetch(const char  );                  //!< wrapper

            //! return value/empty
            template <typename KEY> inline
            const String & operator[](const KEY &key) const
            {
                const String *ps = query(key);
                return ps ? *ps : empty();
            }

            //! recall or on-the-fly create
            template <typename KEY> inline
            String & operator[](const KEY &key)
            {
                return fetch(key);
            }


            //__________________________________________________________________
            //
            //! return string query if options is not NULL, always NULL otherwise
            //__________________________________________________________________
            template <typename NAME> static inline 
            const String * Query(const FormatOptions * const options, const NAME &name) noexcept
            {
                return (0!=options) ? options->query(name) : 0;
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

