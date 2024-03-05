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
                typedef void (Alphabet:: *Emit)(StreamBits &io, const uint8_t);

                explicit Alphabet(const bool useEOS) noexcept;
                virtual ~Alphabet() noexcept;

                void reset() noexcept;
                void reduceFrequencies() noexcept;
                void display(std::ostream &os) const;
                void write(StreamBits &io, const uint8_t byte);


                Unit::List   used;
                Emit         emit;
                Unit * const unit;
                Unit * const nyt;
                Unit * const eos;
                uint32_t     sumf;
                unsigned     nchr; //!< 0..256

                Unit  *      ctrl[Unit::Controls];
                const size_t nctl;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Alphabet);
                static const size_t Required = sizeof(Unit) * Unit::Universe;
                void addCtrl() noexcept; //!< add ctrl to used
                void Init(StreamBits &io, const uint8_t byte);
                void Bulk(StreamBits &io, const uint8_t byte);
                void Full(StreamBits &io, const uint8_t byte);


                void *wksp[ Y_WORDS_GEQ(Required) ];
            };


            

        }

    }

}

#endif
