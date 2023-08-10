//! \file

#ifndef Y_Jive_Pattern_Single_Included
#define Y_Jive_Pattern_Single_Included 1

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
        //! Single char recognition
        //
        //
        //______________________________________________________________________
        class Single : public Pattern
        {
        public:
            //__________________________________________________________________
            //
            //! UUID
            //_________________________________________________________________
            static const uint32_t UUID = Y_FOURCC('S', 'N', 'G', 'L');

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit Single(const uint8_t) noexcept; //!< setup
            virtual ~Single()              noexcept; //!< cleanup

            //__________________________________________________________________
            //
            // Interface
            //__________________________________________________________________
            virtual Pattern *clone() const;           
            virtual bool     takes(Source &);
            virtual void     query(FirstChars &) const;
            virtual size_t   serialize(OutputStream&) const;
            
            //__________________________________________________________________
            //
            // Members
            //__________________________________________________________________
            const uint8_t code; //!< code to match

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Single);
        };
    }

}

#endif
