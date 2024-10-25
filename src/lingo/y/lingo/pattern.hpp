
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
        public Identifiable,
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
            static const char                StrongFn[];   //!< "strong()"
            static const char                UnivocalFn[]; //!< "univocal()"
            static const char                TakesFn[];    //!< "takes()"
            static const char                QueryFn[];    //!< "query()"
            static const char                RegExpFn[];   //!< "regularExpression"
            
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
            virtual bool      takes(Token &token, Source &source)  const = 0; //!< try to take a token from source
            virtual void      query( CharDB &firstChars )          const = 0; //!< query first char for source lookup
            virtual String    regularExpression()                  const = 0; //!< return as regular expression
            virtual bool      univocal()                           const = 0; //!< univocal result
            bool              multiple()                           const;     //!< !univocal()
            virtual bool      strong()                             const = 0; //!< never accepts an empty token
            bool              feeble()                             const;     //!< !strong(), may accept an empty token
            virtual void      viz(OutputStream &fp)       const          = 0; //!< produce GraphViz code


            //__________________________________________________________________
            //
            //
            // Helpers
            //
            //__________________________________________________________________
            friend bool operator==(const Pattern &lhs, const Pattern &rhs);          //!< equality(lhs,rhs)
            static bool        Equality(const Pattern &lhs, const Pattern &rhs);     //!< test equality
            
            static const char *ByteToRegExp(const uint8_t byte) noexcept;            //!< byte to regular expression
            static Pattern    *Read(InputStream &);                                  //!< read serialized pattern
            static Pattern    *Optimize(Pattern * const);                            //!< optimizations
            static Pattern    *IgnoreCase(Pattern * const);                          //!< make case insensitive pattern

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            size_t             emitUUID(OutputStream &fp) const; //!< emit UUID
            void               graphViz(OutputStream &fp) const; //!< standalone GraphViz code from viz()


            //! fetch derived type
            template <typename T> inline
            const T * as() const noexcept
            {
                assert(T::UUID==uuid);
                assert(0!=self);
                return static_cast<const T *>(self);
            }

            //! fetch derived type
            template <typename T> inline
            T * as() noexcept
            {
                assert(T::UUID==uuid);
                assert(0!=self);
                return static_cast<T *>(self);
            }



            static Pattern * Among(const String &);      //!< Or'd chars
            static Pattern * Exact(const String &);      //!< And'd chars
            static Pattern * Among(const char * const);  //!< alias
            static Pattern * Exact(const char * const);  //!< alias

            bool isBasic() const noexcept; //!< one of the basic pattern
            bool isLogic() const noexcept; //!< And, Or, Node
            bool isJoker() const noexcept; //!< Optional, Repeated, Counting
            
            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const uint32_t     uuid; //!< Unique Identifier
            Pattern       *    next; //!< for list
            Pattern       *    prev; //!< for list
        protected:
            void * const       self; //!< store derived address

        private:
            Y_DISABLE_ASSIGN(Pattern);
        };

        //! helper to populate Pattern
#define Y_Lingo_Pattern(TYPE) Coerce(self) = static_cast<TYPE *>(this)

        //! alias
        typedef Pattern::List Patterns;

#define Y_Lingo_Pattern_CallSign(TYPE) \
static const char HostCallSign[] = #TYPE; \
const char * TYPE:: callSign() const noexcept { return HostCallSign; }



    }

}

#endif

