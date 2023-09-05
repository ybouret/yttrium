
//! \file

#ifndef Y_Text_Justify_Included
#define Y_Text_Justify_Included 1

#include "y/config/starting.hpp"
#include "y/string/fwd.hpp"

namespace Yttrium
{
    
    //__________________________________________________________________________
    //
    //
    //
    //! Justify text
    //
    //
    //__________________________________________________________________________
    class Justify
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definition
        //
        //______________________________________________________________________
        static const char SPC = ' '; //!< space

        //! How to justify
        enum Type
        {
            Left,     //!< [text....]
            Center,   //!< [..text..]
            Right     //!< [....text]
        };

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________

        //! setup from msg+len
        Justify(const char *, const size_t, const size_t , const Type = Left) noexcept;

        //! setup from msg
        Justify(const char *, const size_t, const Type = Left) noexcept;

        //! setup from string
        Justify(const Core::String<char> &, const size_t, const Type = Left) noexcept;

        //! copy data
        Justify(const Justify &) noexcept;

        //! cleanup
        ~Justify() noexcept;

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________

        //! display
        template <typename OSTREAM>
        friend inline OSTREAM & operator<<(OSTREAM &os, const Justify &J)
        {
            size_t prolog = 0;
            size_t epilog = 0;
            J.compute(prolog,epilog);
            Pad(os,prolog);
            for(size_t i=0;i<J.size;++i) os << J.text[i];
            Pad(os,epilog);
            return os;
        }

    private:
        Y_DISABLE_ASSIGN(Justify);
        const char * const text;
        const size_t       size;
        const size_t       width;
        const Type         type;

        void compute(size_t &prolog, size_t &epilog) const noexcept;

        template <typename OSTREAM>
        static inline void Pad(OSTREAM &os, size_t n)
        {
            while(n-- > 0) os << SPC;
        }

    };
}

#endif

