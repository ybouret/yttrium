

//! \file

#ifndef Y_Jive_Pattern_Any1_Included
#define Y_Jive_Pattern_Any1_Included 1

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
        //! Any ONE char recognition
        //
        //
        //______________________________________________________________________
        class Any1 : public Pattern
        {
        public:
            //__________________________________________________________________
            //
            //! UUID
            //_________________________________________________________________
            static const uint32_t UUID = Y_FOURCC('A', 'N', 'Y', '1');

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit Any1() noexcept; //!< setup
            virtual ~Any1() noexcept; //!< cleanup

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
            virtual bool     isUnivocal()               const noexcept; //!< false
            virtual bool     isEqualTo(const Pattern &) const noexcept;
            virtual void     reset()                          noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Any1);
        };
    }

}

#endif
