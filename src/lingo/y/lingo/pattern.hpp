
//! \file

#ifndef Y_Lingo_Pattern_Included
#define Y_Lingo_Pattern_Included 1

#include "y/lingo/source.hpp"
#include "y/type/fourcc.hpp"
#include "y/graphviz/vizible.hpp"
#include "y/data/list/cloneable.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        //______________________________________________________________________
        //
        //
        //
        //! Pattern API
        //
        //
        //______________________________________________________________________
        class Pattern :
        public Quantized,
        public Serializable,
        public GraphViz::Vizible
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef ListOfCloneable<Pattern> List;

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

        protected:
            explicit Pattern(const uint32_t t) noexcept;
            explicit Pattern(const Pattern &) noexcept;

        public:
            virtual ~Pattern() noexcept;

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________
            virtual Pattern * clone() const = 0;

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const uint32_t uuid; //!< Unique Identifier
            Pattern       *next; //!< for list
            Pattern       *prev; //!< for list
            
        private:
            Y_DISABLE_ASSIGN(Pattern);
        };

        
    }

}

#endif

