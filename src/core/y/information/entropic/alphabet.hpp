//! \file

#ifndef Y_Information_Entropic_Alphabet_Included
#define Y_Information_Entropic_Alphabet_Included 1

#include "y/calculus/align.hpp"
#include "y/memory/out-of-reach.hpp"
#include "y/data/list/raw.hpp"
#include "y/stream/bits.hpp"

namespace Yttrium
{


    namespace Information
    {

        namespace Entropic
        {

            typedef uint32_t Frequency;
            typedef uint16_t Code;
            typedef uint16_t Bits;
            
            struct Unit
            {
                typedef RawListOf<Unit> List;
                Code       code;
                Bits       bits;
                Frequency  freq;
                Unit      *next;
                Unit      *prev;
            };

            enum OperatingMode
            {
                BlockWise,
                Multiplex
            };

            enum CtrlOffset
            {
                CtrlEOS = 0,
                CtrlNYT = 1
            };


            class Alphabet
            {
            public:
                static const Code     Bytes    = 256;
                static const Code     Ctrls    = 2;
                static const Code     Units    = Bytes + Ctrls;
                static const Code     EOS      = Bytes + CtrlEOS;
                static const Code     NYT      = Bytes + CtrlNYT;
                static const unsigned Required = Units * sizeof(Unit);
                typedef void (Alphabet::*Emit)(StreamBits &, Unit &);

                explicit Alphabet(const OperatingMode how,
                                  const bool          verbosity=false) noexcept;
                virtual ~Alphabet() noexcept;

                //! emit byte to io, no model
                void write(StreamBits &io, const uint8_t byte);

                //! send EOS and fill io to byte boundary
                void flush(StreamBits &io);

                //! reset all alphabet
                void reset() noexcept;



            protected:
                Unit       * const unit;
                Emit               emit;
                Unit       * const eos;
                Unit       * const nyt;
                Unit::List         used;
                
            public:
                const OperatingMode mode;
                bool                verbose;
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Alphabet);

                void *wksp[ Y_WORDS_GEQ(Required) ];

                void send(StreamBits &io, const Unit &u); //!< send unit to io
                void rank(Unit &u)              noexcept; //!< keep ranked used
                void pushControls()             noexcept; //!< push initial controls
                void emitInit(StreamBits &io, Unit &u);   //!< emit first unit
                void emitBulk(StreamBits &io, Unit &u);   //!< emit bulk unit
                void emitFull(StreamBits &io, Unit &u);   //!< emit unit with full Alphabet



            };

            

        }

    }

}

#endif
