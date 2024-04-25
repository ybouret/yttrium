//! \file

#ifndef Y_Jive_Pattern_Included
#define Y_Jive_Pattern_Included 1

#include "y/jive/token.hpp"
#include "y/stream/serializable.hpp"
#include "y/graphviz/vizible.hpp"
#include "y/ptr/arc.hpp"

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
        class Pattern : public Object, public Counted, public Token, public Serializable, public GraphViz:: Vizible
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
            virtual bool      takes(Source &)                           = 0;
            virtual void      query(FirstChars &)        const          = 0; //!< query first chars
            virtual Pattern * clone()                    const          = 0; //!< perform deep copy
            virtual void      reset()                          noexcept = 0; //!< reset content
            virtual bool      isUnivocal()               const noexcept = 0; //!< only one possible token can be accepted
            bool              isMultiple()               const noexcept;     //!< multiple tokens can match the pattern
            virtual bool      isFragile()                const noexcept = 0; //!< accept empty token
            bool              isRegular()                const noexcept;     //!< !isFragile()
            virtual bool      isEqualTo(const Pattern &) const noexcept = 0; //!< check equality
            virtual void      viz(OutputStream &fp)      const          = 0; //!< output visual
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

            //__________________________________________________________________
            //
            //! transform into case ignorinc pattern
            //__________________________________________________________________
            static Pattern * IgnoreCase(Pattern *);

            //__________________________________________________________________
            //
            // create logical patterns
            //__________________________________________________________________
            static Pattern *Exact(const char *   ); //!< And[text]
            static Pattern *Exact(const String & ); //!< And[text]
            static Pattern *Among(const char *   ); //!< Or[text]
            static Pattern *Among(const String & ); //!< Or[text]
            static Pattern *Avoid(const char *   ); //!< None[text]
            static Pattern *Avoid(const String & ); //!< None[text]

            //__________________________________________________________________
            //
            // Testing
            //__________________________________________________________________
            friend bool operator==(const Pattern &, const Pattern &) noexcept; //!< test equality
            friend bool operator!=(const Pattern &, const Pattern &) noexcept; //!< test difference
            
            bool isBasic() const noexcept; //!< check is it's a basic pattern


            //__________________________________________________________________
            //
            // saving
            //__________________________________________________________________
            void graphViz(OutputStream &) const;    //!< call viz
            static String ToRegExp(const String &); //!< convert a string into its exact matching pattern


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

        //______________________________________________________________________
        //
        //
        //! alias
        //
        //______________________________________________________________________
        typedef ArcPtr<Pattern> Motif;
        
    }

}

#endif
