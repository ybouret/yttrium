
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
        //______________________________________________________________________
        //
        //
        //
        //! Format interface
        //
        //
        //______________________________________________________________________
        class Format : public Object, public Counted
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef ArkPtr<String,Format> Handle; //!< alias

        protected:
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
           
            //! setup name
            template <typename UID> inline
            explicit Format(const UID &uid) : name(uid) {}

        public:
            //! cleanup
            virtual ~Format() noexcept;

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            bool          matches(const String &      ) const noexcept; //!< matches extension
            bool          matches(const char   * const) const noexcept; //!< matches extension
            const String &key()                         const noexcept; //!< key for database

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________

            //! name of the format, a.k.a key()
            const String  name;

        protected:

            //! helper to match case insensitive extensions
            static bool CaseInsensitiveMatch(const char * const lhs, const size_t lsz,
                                             const char * const rhs, const size_t rsz) noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Format);
            virtual bool matches(const char * const ext, const size_t len) const noexcept = 0;
        };
    }

}

#endif

