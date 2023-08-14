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
            static const uint32_t     UUID = Y_FOURCC('R','E','P','_'); //!< UUID
            static const uint32_t     ZOM  = Y_FOURCC('Z','O','M','_'); //!< Zero Or More
            static const uint32_t     OOM  = Y_FOURCC('O','O','M','_'); //!< One Or More

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Repeating(const Repeating&);                       //!< duplicate
            virtual ~Repeating() noexcept;                              //!< cleanup
            static   Pattern *Make(const size_t nmin, Pattern *);       //!< take care of pattern
            static   Pattern *Make(const size_t nmin, const Pattern &); //!< take care of clone

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
            virtual bool     isEqualTo(const Pattern &) const noexcept;
            virtual void     viz(OutputStream &fp)      const;

            static Pattern *ZeroOrMore(Pattern *      p); //!< Make(0,p)
            static Pattern *ZeroOrMore(const Pattern &p); //!< Make(0,p)

            static Pattern *OneOrMore(Pattern *      p);  //!< Make(1,p)
            static Pattern *OneOrMore(const Pattern &p);  //!< Make(1,p)


            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const size_t atLeast; //!< minimal valid count


        private:
            Y_DISABLE_ASSIGN(Repeating);
            explicit Repeating(const size_t nmin, Pattern *); //!< setup directly
        };






        

    }

}

#endif
