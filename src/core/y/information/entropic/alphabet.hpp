//! \file

#ifndef Y_Information_Entropic_Alphabet_Included
#define Y_Information_Entropic_Alphabet_Included 1

#include "y/information/entropic/model.hpp"
#include "y/calculus/align.hpp"
#include "y/data/list/raw.hpp"

namespace Yttrium
{


    namespace Information
    {

        namespace Entropic
        {

            class Alphabet
            {
            public:
                explicit Alphabet(const bool useEOS) noexcept;
                virtual ~Alphabet() noexcept;

                void reset() noexcept;
                void reduceFrequencies() noexcept;
                void display(std::ostream &os) const;

                Unit::List   used;
                Unit * const unit;
                Unit * const nyt;
                Unit * const eos;
                uint32_t     sumf;

                Unit  *      ctrl[Unit::Controls];
                const size_t nctl;

            private:
                Y_DISABLE_ASSIGN(Alphabet);
                static const size_t Required = sizeof(Unit) * Unit::Universe;
                void addCtrl() noexcept;

                void *wksp[ Y_WORDS_GEQ(Required) ];
            };


            

        }

    }

}

#endif
