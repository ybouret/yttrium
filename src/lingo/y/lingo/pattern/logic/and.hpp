
//! \file

#ifndef Y_Lingo_Logic_And_Included
#define Y_Lingo_Logic_And_Included 1

#include "y/lingo/pattern/logic.hpp"

namespace Yttrium
{
    namespace Lingo
    {

        class And : public Logic
        {
        public:
            static const uint32_t UUID = Y_FOURCC('[','&', '&', ']');

            explicit And() noexcept;
            virtual ~And() noexcept;
            And(const And &);

            virtual Pattern *clone() const;
            virtual bool     strong()       const noexcept; //!< if size>0 and all strong
            virtual bool     univocal()     const noexcept; //!< false is one is feeble or not univocal
            virtual String   regularExpression()     const; //!< (all)
            virtual void     query(CharDB &fc)       const; //!< up to first strong
            virtual bool     takes(Token &,Source &) const; //!< all are taken

        private:
            Y_DISABLE_ASSIGN(And);
        };
    }

}

#endif
