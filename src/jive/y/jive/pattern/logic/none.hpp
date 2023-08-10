
//! \file

#ifndef Y_Jive_Pattern_None_Included
#define Y_Jive_Pattern_None_Included 1

#include "y/jive/pattern/logic/compound.hpp"
#include "y/type/fourcc.hpp"

namespace Yttrium
{
    namespace Jive
    {

        //______________________________________________________________________
        //
        //
        //
        //! Logical Or: must match one of the patterns, in order
        //
        //
        //______________________________________________________________________
        class None : public Compound
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const uint32_t UUID = Y_FOURCC('_','!','=','_'); //!< UUID

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit None()   noexcept;   //!< setup empty
            explicit None(const None &);  //!< copy
            virtual ~None()   noexcept;   //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            virtual Pattern *clone() const;
            virtual bool     takes(Source &);
            virtual void     query(FirstChars &) const;

            //! false return first non matching char if any
            virtual bool     isFragile() const noexcept;

        private:
            Y_DISABLE_ASSIGN(None);
        };
    }

}

#endif
