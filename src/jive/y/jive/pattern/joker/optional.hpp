
//! \file

#ifndef Y_Jive_Pattern_Optional_Included
#define Y_Jive_Pattern_Optional_Included 1

#include "y/jive/pattern/joker/guest.hpp"
#include "y/type/fourcc.hpp"

namespace Yttrium
{
    namespace Jive
    {
        //______________________________________________________________________
        //
        //
        //
        //! Optional pattern recognition
        //
        //
        //______________________________________________________________________
        class Optional : public Guest
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definition
            //
            //__________________________________________________________________
            static const uint32_t UUID = Y_FOURCC('O','P','T','_'); //!< UUID

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Optional(const Optional&);        //!< duplicate
            virtual ~Optional() noexcept;              //!< cleanup
            static   Pattern *From(Pattern *);        //!< create from pattern
            static   Pattern *From(const Pattern &);  //!< create from cloned
            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            virtual Pattern *clone() const;                   //!< clone
            virtual bool     takes(Source &);                 //!< try guest, return true
            virtual bool     isFragile() const noexcept;      //!< true
            virtual size_t   serialize(OutputStream&) const;  //!< uuid+guest
            virtual bool     isEqualTo(const Pattern &) const noexcept;
            virtual void     viz(OutputStream &fp)      const;
            virtual bool     isUnivocal()               const noexcept; //!< transfer to guest

        private:
            Y_DISABLE_ASSIGN(Optional);
            explicit Optional(Pattern *); //!< setup directly
        };


    }

}

#endif
