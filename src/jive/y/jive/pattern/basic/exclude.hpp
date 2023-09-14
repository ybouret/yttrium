
//! \file

#ifndef Y_Jive_Pattern_Exclude_Included
#define Y_Jive_Pattern_Exclude_Included 1

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
        //! Exclude one char
        //
        //
        //______________________________________________________________________
        class Exclude : public Pattern
        {
        public:
            //__________________________________________________________________
            //
            //! UUID
            //_________________________________________________________________
            static const uint32_t UUID = Y_FOURCC('E', 'X', 'C', 'L');

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit Exclude(const uint8_t)  noexcept; //!< setup
            virtual ~Exclude()               noexcept; //!< cleanup
            explicit Exclude(const Exclude &) noexcept; //!< copy
            //__________________________________________________________________
            //
            // Interface
            //__________________________________________________________________
            virtual bool     takes(Source &);
            virtual Pattern *clone()                    const;
            virtual void     query(FirstChars &)        const;
            virtual size_t   serialize(OutputStream&)   const;
            virtual void     viz(OutputStream &fp)      const;
            virtual bool     isFragile()                const noexcept; //!< false
            virtual bool     isEqualTo(const Pattern &) const noexcept;
            virtual bool     isUnivocal()               const noexcept;
            virtual void     reset()                          noexcept;

            //__________________________________________________________________
            //
            // Members
            //__________________________________________________________________
            const uint8_t code; //!< code to exlcude

        private:
            Y_DISABLE_ASSIGN(Exclude);
        };
    }

}

#endif
