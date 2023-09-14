//! \file

#ifndef Y_Jive_Pattern_Range_Included
#define Y_Jive_Pattern_Range_Included 1

#include "y/jive/pattern.hpp"
#include "y/type/fourcc.hpp"

namespace Yttrium
{
    namespace Jive
    {
        //______________________________________________________________________
        //
        //
        //
        //! Range of chars recognition
        //
        //
        //______________________________________________________________________
        class Range : public Pattern
        {
        public:
            //__________________________________________________________________
            //
            //! UUID
            //_________________________________________________________________
            static const uint32_t UUID = Y_FOURCC('R', 'N', 'G', 'E');

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit Range(const uint8_t, const uint8_t) noexcept; //!< setup
            explicit Range(const Range &other)           noexcept; //!< copy
            virtual ~Range()                             noexcept; //!< cleanup

            //__________________________________________________________________
            //
            // Interface
            //__________________________________________________________________
            virtual Pattern *clone() const;
            virtual bool     takes(Source &);
            virtual void     query(FirstChars &) const;
            virtual void     reset() noexcept;
            virtual size_t   serialize(OutputStream&) const;
            virtual bool     isFragile() const noexcept; //!< false
            virtual bool     isEqualTo(const Pattern &) const noexcept;
            virtual void     viz(OutputStream &fp)      const;
            virtual bool     isUnivocal() const noexcept;
            //__________________________________________________________________
            //
            // Members
            //__________________________________________________________________
            const uint8_t lower; //!< lower code to match
            const uint8_t upper; //!< upper code to match

        private:
            Y_DISABLE_ASSIGN(Range);
        };
    }

}

#endif
