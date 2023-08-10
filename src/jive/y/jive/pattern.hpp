//! \file

#ifndef Y_Jive_Pattern_Included
#define Y_Jive_Pattern_Included 1

#include "y/jive/token.hpp"
#include "y/io/serializable.hpp"

namespace Yttrium
{
    namespace Jive
    {

        class Source;
        class FirstChars;
        
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
        class Pattern : public Object, public Token, public Serializable
        {
        public:
            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //__________________________________________________________________
            //
            //! try to accept next source content
            /**
             - token must be empty on call
             - token must be left empty on failure
             */
            //__________________________________________________________________
            virtual bool      takes(Source &)            = 0;
            virtual void      query(FirstChars &)  const = 0; //!< query first chars
            virtual Pattern * clone()              const = 0; //!< perform deep copy
            virtual void      reset()           noexcept = 0; //!< reset content
            virtual bool      isFragile() const noexcept = 0; //!< accept empty token
            bool              isRegular() const noexcept;     //!< !isFragile()

            //__________________________________________________________________
            //
            //! conversion
            //__________________________________________________________________
            template <typename T> inline
            T *as() noexcept
            {
                assert(T::UUID==uuid);
                assert(0!=self);
                return static_cast<T*>(self);
            }

            //__________________________________________________________________
            //
            //! conversion
            //__________________________________________________________________
            template <typename T> inline
            const T *as() const noexcept
            {
                assert(T::UUID==uuid);
                assert(0!=self);
                return static_cast<const T*>(self);
            }

            //__________________________________________________________________
            //
            //! Read serialized
            //__________________________________________________________________
            static Pattern * ReadFrom(InputStream&);

            //__________________________________________________________________
            //
            //! perform various optimization
            //__________________________________________________________________
            static Pattern * Optimize(Pattern *);
            static Pattern * IgnoreCase(Pattern *);
            
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
            explicit Pattern(const Pattern &) noexcept; //!< copy uuid
            
        private:
            Y_DISABLE_ASSIGN(Pattern);
        };

    }

}

#endif
