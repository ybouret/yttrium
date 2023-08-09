//! \file

#ifndef Y_Apex_Number_Included
#define Y_Apex_Number_Included 1

#include "y/object.hpp"
#include "y/io/serializable.hpp"

#include <iosfwd>

namespace Yttrium
{
    namespace IO     { class Chars; }

    namespace Apex
    {
        
        //______________________________________________________________________
        //
        //
        //
        //! base class for any Apex number
        //
        //
        //______________________________________________________________________
        class Number : public Object, public Serializable
        {
        protected:
            explicit Number() noexcept;
            
        public:
            virtual ~Number() noexcept; //!< cleanup

            //! transform a cache into a C-string and send it to output
            static void Display(std::ostream &, IO::Chars &);
            
        private:
            Y_DISABLE_ASSIGN(Number);
        };
    }
}

#endif

