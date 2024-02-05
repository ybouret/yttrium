
//! \file

#ifndef Y_Field_Sketch_Included
#define Y_Field_Sketch_Included 1

#include "y/field/meta-key.hpp"

namespace Yttrium
{
    namespace Field
    {

        //______________________________________________________________________
        //
        //
        //
        //! Agnostic Field
        //
        //
        //______________________________________________________________________
        class Sketch 
        {
        public:
            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________
            virtual const MetaKey & key() const noexcept = 0; //!< get metaKey
            
            
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            virtual ~Sketch() noexcept; //!< cleanup
        protected:
            explicit Sketch() noexcept; //!< setup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Sketch);
        };

    }

}

#endif
