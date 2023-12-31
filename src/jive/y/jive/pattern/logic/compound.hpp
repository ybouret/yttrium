//! \file

#ifndef Y_Jive_Pattern_Compound_Included
#define Y_Jive_Pattern_Compound_Included 1

#include "y/jive/pattern.hpp"
#include "y/data/list/cloneable.hpp"

namespace Yttrium
{
    namespace Jive
    {
        //______________________________________________________________________
        //
        //
        //! list of cloneable patterns
        //
        //______________________________________________________________________
        typedef ListOfCloneable<Pattern> Patterns;

        //______________________________________________________________________
        //
        //
        //
        //! a Compound is a Pattern with a list of patterns
        //
        //
        //______________________________________________________________________
        class Compound : public Pattern
        {
        public:
            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            virtual void   reset()                noexcept;  //!< recursive reset
            void           noDuplicate()          noexcept;  //!< remove duplicate patterns
            virtual size_t serialize(OutputStream &) const;  //!< uuid+num patterns+patterns
            void           retrieve(InputStream &);          //!< load num patterns+patterns

            Compound & operator<<( const Pattern & ); //!< append clone of argument
            Compound & operator<<( Pattern *);        //!< append argument


            void feed(const char *, const size_t); //!< feed with Singles
            void feed(const char *);               //!< feed with Singles
            void feed(const String &);             //!< feed with Singles

            void add(const uint8_t);                //!< add a Single
            void add(const uint8_t, const uint8_t); //!< add a Range


            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            Patterns      patterns; //!< operands
            const size_t &count;    //!< patterns.size

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            virtual ~Compound() noexcept;  //!< cleanup
        protected:
            explicit Compound(const uint32_t  ) noexcept; //!< setup empty
            explicit Compound(const Compound &);          //!< copy pattern+patterns

            //! checking all patterns are equals
            bool hasSamePatternsThan(const Compound &) const noexcept;



            //! output and link patterns
            virtual void vizLink(OutputStream &fp)     const;


        private:
            Y_DISABLE_ASSIGN(Compound);
        };

    }

}

#endif
