
//! \file

#ifndef Y_Chemical_Formula_Included
#define Y_Chemical_Formula_Included 1

#include "y/chemical/type/lang.hpp"
#include "y/chemical/type/memory.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //! formula representation as tree
        typedef ArcPtr<const XNode> XTree;

        //______________________________________________________________________
        //
        //
        //
        //! Base class for XNode to String conversion
        //
        //
        //______________________________________________________________________
        class Formula :  public GraphViz::Vizible
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            class Linker;

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Formula(const XTree  &     code); //!< create from parsed XNode
            explicit Formula(const String &     expr); //!< create from expression
            explicit Formula(const char * const expr); //!< create from expression
            virtual ~Formula()       noexcept;         //!< cleanup
            Formula(const Formula &) noexcept;         //!< duplicate

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void viz(OutputStream &     fp,
                     const char * const color,
                     const char * const style) const;

            void graphViz(OutputStream &fp) const;

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const XName   uuid;  //!< unique ID
            const String &name;  //!< *uuid
            const XTree   tree;  //!< internal representation
            const int     z;     //!< algebraic charge

        private:
            Y_DISABLE_ASSIGN(Formula);

        };

    }

}

#endif

