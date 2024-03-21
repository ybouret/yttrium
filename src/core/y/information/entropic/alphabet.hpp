//! \file

#ifndef Y_Information_Entropic_Alphabet_Included
#define Y_Information_Entropic_Alphabet_Included 1

#include "y/information/entropic/symbol.hpp"
#include "y/calculus/align.hpp"

namespace Yttrium
{


    namespace Information
    {

        namespace Entropic
        {
            //__________________________________________________________________
            //
            //
            //
            //! Alphabet of Units
            //
            //
            //__________________________________________________________________
            class Alphabet
            {
            public:
                enum Mode
                {
                    Precompiling, //!< no  EOS, no  NYT
                    SingleStream, //!< no  EOS, use NYT
                    MultiStreams  //!< use EOS, use NYT
                };
                
                static const size_t Required = Symbol::Universe * sizeof(Symbol); //!< RequiredBytes

                explicit Alphabet(const Mode mode) noexcept;
                virtual ~Alphabet() noexcept;

                void reset() noexcept;
                void reduceFrequencies() noexcept;
                void display(std::ostream &os) const;

                const Symbol::List & operator*()  const noexcept;
                const Symbol::List * operator->() const noexcept;

                const Symbol & NYT() const noexcept;
                const Symbol & EOS() const noexcept;

                const Symbol & operator()(const uint8_t);




            private:
                Y_DISABLE_COPY_AND_ASSIGN(Alphabet);
                Symbol::List   symbols;
                size_t         active;
                Symbol * const symbol;
                Symbol * const nyt;
                Symbol * const eos;
                Symbol *       control[Symbol::Controls];
                size_t         controls;
                void *         workspace[ Y_WORDS_GEQ(Required) ];

                void initControls(const Mode) noexcept;
                void pushControls() noexcept;
            };


            

        }

    }

}

#endif
