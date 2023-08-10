//! \file

#ifndef Y_Jive_Pattern_Repeating_Included
#define Y_Jive_Pattern_Repeating_Included 1

#include "y/jive/pattern/joker/guest.hpp"
#include "y/type/fourcc.hpp"

namespace Yttrium
{
    namespace Jive
    {
        //______________________________________________________________________
        //
        //
        //
        //! Repeating pattern recognition
        //
        //
        //______________________________________________________________________
        class Repeating : public Guest
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definition
            //
            //__________________________________________________________________
            static const char * const CallSign; //!< Repeating
            static const uint32_t     UUID = Y_FOURCC('R','E','P',':'); //!< UUID
            static const uint32_t     ZOM  = Y_FOURCC('Z','O','M',':'); //!< Zero Or More
            static const uint32_t     OOM  = Y_FOURCC('O','O','M',':'); //!< One Or More

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Repeating(const size_t nmin, const Pattern  &); //!< setup from clone
            explicit Repeating(const size_t nmin, Pattern        *); //!< setup directly
            explicit Repeating(const Repeating&);                    //!< duplicate
            virtual ~Repeating() noexcept;                           //!< cleanup

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            virtual Pattern *clone() const;                   //!< clone
            virtual bool     takes(Source &);                 //!< try guest, return true
            virtual bool     isFragile() const noexcept;      //!< atLeast==0 or motif->isFragile()
            virtual size_t   serialize(OutputStream&) const;  //!< uuid[+atLeast]+guest

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const size_t atLeast;


        private:
            Y_DISABLE_ASSIGN(Repeating);
        };

        template <size_t N>
        class SpecificOrMore : public Repeating
        {
        public:
            inline explicit SpecificOrMore(const Pattern &p) :        Repeating(N,p) {}
            inline explicit SpecificOrMore(Pattern *      p) :        Repeating(N,p) {}
            inline explicit SpecificOrMore(const SpecificOrMore &_) : Repeating(_)   {}
            inline virtual ~SpecificOrMore() noexcept                                {}
        private:
            Y_DISABLE_ASSIGN(SpecificOrMore);
        };

        typedef SpecificOrMore<0> ZeroOrMore;
        typedef SpecificOrMore<1> OneOrMore;


    }

}

#endif
