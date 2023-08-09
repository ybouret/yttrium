//! \file

#ifndef Y_Jive_Pattern_Included
#define Y_Jive_Pattern_Included 1

#include "y/jive/source.hpp"
#include "y/type/fourcc.hpp"

namespace Yttrium
{
    namespace Jive
    {

        //! helper to declare internal pattern type
#define Y_PATTERN(TYPE) self = static_cast<TYPE*>(this)

        //______________________________________________________________________
        //
        //
        //
        //! Pattern recognition
        //
        //
        //______________________________________________________________________
        class Pattern : public Object, public Token
        {
        public:
            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            virtual Pattern * clone() const = 0; //!< perform deep copy

            //__________________________________________________________________
            //
            //! try to accept next source content
            /**
             - token must be empty on call
             - token must be left empty on failure
             */
            //__________________________________________________________________
            virtual bool      accepts(Source &source) = 0;


            //! conversion
            template <typename T> inline
            T *as() noexcept
            {
                assert(T::UUID==uuid);
                assert(0!=self);
                return static_cast<T*>(self);
            }

            //! conversion
            template <typename T> inline
            const T *as() const noexcept
            {
                assert(T::UUID==uuid);
                assert(0!=self);
                return static_cast<const T*>(self);
            }

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
        public:    Pattern       *next; //!< for list
        public:    Pattern       *prev; //!< for list
        protected: void          *self; //!< pointer to base class
        public:    const uint32_t uuid; //!< identifier

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            virtual ~Pattern() noexcept; //!< cleanup
        protected:
            explicit Pattern(const uint32_t ) noexcept; //!< setup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Pattern);
        };

    }

}

#endif
