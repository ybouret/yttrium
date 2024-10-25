
//! \file

#ifndef Y_Lingo_Logic_Included
#define Y_Lingo_Logic_Included 1

#include "y/lingo/pattern.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        //______________________________________________________________________
        //
        //
        //
        //! Logic is a Pattern and a list of operands
        //
        //
        //______________________________________________________________________
        class Logic : public Pattern, public Pattern::List
        {
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
        protected:
            explicit Logic(const uint32_t t) noexcept; //!< setup
            Logic(const Logic &);                      //!< copy


        public:
            virtual ~Logic() noexcept;

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________
            virtual size_t serialize(OutputStream &fp) const;
            virtual void   query(CharDB &fc)        const; //!< [Pattern] check size and _query(fc)
            virtual bool   strong()                 const; //!< [Pattern] check size and _strong()
            virtual String regularExpression()      const; //!< [Pattern] check size and _regexp()
            virtual bool   univocal()               const; //!< [Pattern] check size and _univocal()
            virtual bool   takes(Token &, Source &) const; //!< [Pattern] check size and _takes()
            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void    vizLink(OutputStream &)        const;              //!< produce link to children
            Logic & operator<<(Pattern * const) noexcept;              //!< helper to push new patter
            Logic & add(const uint8_t single);                         //!< add new single
            Logic & add(const uint8_t lower, const uint8_t upper);     //!< add new range
            static bool HaveSameContent(const Logic &, const Logic &); //!< patter-wise comparison

            void optimizing();                         //!< optimize each pattern
            void ignoreCase();                         //!< ignoreCase for each pattern
            void noMultiple();                         //!< remove multiple pattern


        private:
            Y_DISABLE_ASSIGN(Logic);
            virtual void   _query(CharDB &firstChars)  const = 0;
            virtual bool   _strong()                   const = 0;
            virtual String _regexp()                   const = 0;
            virtual bool   _univocal()                 const = 0;
            virtual bool   _takes(Token &, Source &)   const = 0;;
        };

    }

}

#endif

