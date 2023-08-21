
//! \file

#ifndef Y_Text_Justify_Included
#define Y_Text_Justify_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{

    namespace Core { template <typename> class String; }

    class Justify
    {
    public:
        static const char SPC = ' ';
        enum Type
        {
            Left,
            Center,
            Right
        };

        Justify(const char *, const size_t, const size_t , const Type = Left) noexcept;
        Justify(const char *, const size_t, const Type = Left) noexcept;
        Justify(const Core::String<char> &, const size_t, const Type = Left) noexcept;

        Justify(const Justify &) noexcept;
        ~Justify() noexcept;

        template <typename OSTREAM>
        friend inline OSTREAM & operator<<(OSTREAM &os, const Justify &J)
        {
            size_t prolog = 0;
            size_t epilog = 0;
            J.compute(prolog,epilog);
            for(size_t i=prolog;i>0;--i) os << SPC;
            for(size_t i=0;i<J.size;++i) os << J.text[i];
            for(size_t i=epilog;i>0;--i) os << SPC;
            return os;
        }

    private:
        Y_DISABLE_ASSIGN(Justify);
        const char * const text;
        const size_t       size;
        const size_t       width;
        const Type         type;

        void compute(size_t &prolog, size_t &epilog) const noexcept;


    };
}

#endif

