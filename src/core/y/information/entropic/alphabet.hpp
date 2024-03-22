//! \file

#ifndef Y_Information_Entropic_Alphabet_Included
#define Y_Information_Entropic_Alphabet_Included 1

#include "y/information/entropic/model.hpp"
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
                static const char * ModeText(const Mode) noexcept;
                static const size_t Required = Symbol::Universe * sizeof(Symbol); //!< RequiredBytes

                explicit Alphabet(const Mode m) noexcept;
                virtual ~Alphabet() noexcept;

                void reset() noexcept;
                void reduceFrequencies() noexcept;
                void display(std::ostream &os) const;

                const char *         modeText()   const noexcept;
                const Symbol::List & operator*()  const noexcept;
                const Symbol::List * operator->() const noexcept;

                const Symbol & operator[](const uint8_t) const noexcept;

                void update(const Symbol &symb) noexcept; //!< update chosen symbol
                void commit(Model &)            noexcept; //!< symbols.size>0


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Alphabet);
                Symbol::List     symbols; //!< list of symbols+controls
                Symbol::FreqType sumFreq; //!< sum of all frequencies
                size_t           active;  //!< number of active
                Symbol * const   symbol;
                const Mode       mode;
                Symbol * const   nyt;
                Symbol * const   eos;
                void *           workspace[ Y_WORDS_GEQ(Required) ];

                void pushControls() noexcept;
                void insertSymbol(Symbol * const) noexcept;
                void updateSymbol(Symbol * const) noexcept;
                void monitorFreqs() noexcept;
            };


            

        }

    }

}

#endif
