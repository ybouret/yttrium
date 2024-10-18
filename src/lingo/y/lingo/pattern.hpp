
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

        class CharDB;

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
            typedef ListOfCloneable<Pattern> List; //!< alias

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

        protected:
            explicit Pattern(const uint32_t t) noexcept; //!< setup with uuid
            explicit Pattern(const Pattern  &) noexcept; //!< copy

        public:
            virtual ~Pattern() noexcept;

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________
            virtual Pattern * clone() const = 0; //!< full clone
            virtual bool      takes(Token &token, Source &source) const = 0;
            virtual void      query( CharDB &firstChars )         const = 0;
            virtual String    regularExpression()                 const = 0;
            virtual bool      univocal()                  const noexcept = 0; //!< univocal result
            bool              multiple()                  const noexcept;     //!< !univocal()
            virtual bool      strong()                    const noexcept = 0; //!< never accepts an empty token
            bool              feeble()                    const noexcept;     //!< !strong(), may accept an empty token


            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            size_t emitUUID(OutputStream &fp) const;
            static const char *ByteToRegExp(const uint8_t byte) noexcept;

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

