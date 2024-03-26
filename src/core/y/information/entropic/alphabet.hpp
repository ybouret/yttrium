//! \file

#ifndef Y_Information_Entropic_Alphabet_Included
#define Y_Information_Entropic_Alphabet_Included 1

#include "y/information/entropic/model.hpp"
#include "y/calculus/align.hpp"
#include "y/memory/buffer/ro.hpp"

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
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
               
                //! building mode
                enum Mode
                {
                    Precompiling, //!< no  EOS, no  NYT
                    SingleStream, //!< no  EOS, use NYT
                    MultiStreams  //!< use EOS, use NYT
                };
                static const char * ModeText(const Mode) noexcept;                //!< human readadle mode
                static const size_t Required = Symbol::Universe * sizeof(Symbol); //!< RequiredBytes

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit Alphabet(const Mode m) noexcept; //!< setup
                virtual ~Alphabet()             noexcept; //!< cleanup

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________
             
                void reset()                noexcept; //!< reset all
                void reduceFrequencies()    noexcept; //!< reduce all frequencies
                void display(std::ostream &os) const; //!< display info

                const char *         modeText()   const noexcept; //!< human readable
                const Symbol::List & operator*()  const noexcept; //!< access
                const Symbol::List * operator->() const noexcept; //!< access

                const Symbol & operator[](const uint8_t) const noexcept; //!< access symbol
                void update(const Symbol &symb)                noexcept; //!< update chosen symbol
                void commit(Model &)                           noexcept; //!< symbols.size>0

                //! count current bits required to encode buffer and deduce bytes
                size_t outputBytesFor(const Memory::ReadOnlyBuffer &) const;


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
