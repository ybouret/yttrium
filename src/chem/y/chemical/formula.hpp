
//! \file

#ifndef Y_Chemical_Formula_Included
#define Y_Chemical_Formula_Included 1

#include "y/chemical/type/lang.hpp"
#include "y/chemical/type/memory.hpp"

namespace Yttrium
{
    namespace Chemical
    {


        typedef ArcPtr<const XNode> XCode;

        //______________________________________________________________________
        //
        //
        //
        //! Base class for XNode to String conversion
        //
        //
        //______________________________________________________________________
        class Formula
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definition
            //
            //__________________________________________________________________
            class Linker;

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Formula(const XCode  &     code); //!< create from parsed XNode
            explicit Formula(const String &     expr); //!< create from expression
            explicit Formula(const char * const expr); //!< create from expression
            virtual ~Formula() noexcept;               //!< cleanup
            Formula(const Formula &_) noexcept;

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const Title      title; //!< shared name
            const XCode      xcode; //!< internal representation
            const int        z;     //!< algebraic charge

        private:
            Y_DISABLE_ASSIGN(Formula);

        };

    }

}

#endif

