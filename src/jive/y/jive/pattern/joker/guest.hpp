//! \file

#ifndef Y_Jive_Pattern_Guest_Included
#define Y_Jive_Pattern_Guest_Included 1

#include "y/jive/pattern.hpp"
#include "y/ptr/auto.hpp"

namespace Yttrium
{
    namespace Jive
    {
        //______________________________________________________________________
        //
        //
        //
        //! Guest for a Pattern
        //
        //
        //______________________________________________________________________
        class Guest : public Pattern
        {
        public:
            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            virtual void reset()             noexcept; //!< release and propagate reset
            virtual void query(FirstChars &) const;    //!< forward call
            void         optimize();                   //!< forward call

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            virtual ~Guest() noexcept; //!< cleanup
        protected:
            explicit Guest(const uint32_t, const Pattern &);          //!< uuid+clone
            explicit Guest(const uint32_t, Pattern       *) noexcept; //!< uuid+pattern
            explicit Guest(const Guest &other);                       //!< duplicate

            bool hasSameMotifThan(const Guest &) const noexcept; //!< forward testing
            void vizLink(OutputStream &fp)       const;          //!< output and link motif

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            AutoPtr<Pattern> motif; //!< guest pattern

        private:
            Y_DISABLE_ASSIGN(Guest);
        };


    }

}

#endif
