//! \file

#ifndef Y_Information_Entropic_Alphabet_Included
#define Y_Information_Entropic_Alphabet_Included 1

#include "y/calculus/align.hpp"
#include "y/data/list/raw.hpp"
#include "y/stream/bits.hpp"
#include "y/type/ints.hpp"

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
            // Types
            //
            //
            //__________________________________________________________________
            typedef uint32_t Frequency; //!< hold frequencies
            typedef uint16_t Code;      //!< hold codes
            typedef uint16_t Bits;      //!< hold bits for codes

            //__________________________________________________________________
            //
            //
            //
            //! C-Style unit
            //
            //
            //__________________________________________________________________
            struct Unit
            {
                typedef RawListOf<Unit> List; //!< list of unit

                Code       code; //!< current code
                Bits       bits; //!< bits for code
                Frequency  freq; //!< frequencies
                Unit      *next; //!< for list
                Unit      *prev; //!< for list
            };

            //__________________________________________________________________
            //
            //
            //
            //! Operating Modes
            //
            //
            //__________________________________________________________________
            enum OperatingMode
            {
                BlockWise, //!< no  EOS
                Multiplex  //!< use EOS
            };


            //__________________________________________________________________
            //
            //
            //
            //! Control codes offset
            //
            //
            //__________________________________________________________________
            enum CtrlOffset
            {
                CtrlEOS = 0, //!< for End Of Stream
                CtrlNYT = 1  //!< for Not Yet Transmitted
            };

            //__________________________________________________________________
            //
            //
            //
            //! Alphabet Bookkeeping
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
                static const Code     Bytes    = 256;                   //!< number of data bytes
                static const Code     Ctrls    = 2;                     //!< number of control units
                static const Code     Units    = Bytes + Ctrls;         //!< number of units
                static const Code     EOS      = Bytes + CtrlEOS;       //!< index of End Of Stream
                static const Code     NYT      = Bytes + CtrlNYT;       //!< index of Not Yet Transmitted
                typedef void (Alphabet::*Emit)(StreamBits &, Unit &);   //!< emit prototype
                static const Frequency MaxSumFreq = IntegerFor<Frequency>::Maximum;

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________

                //! setup
                /**
                 \param how        set if BlockWise/Multiplex
                 \param verbosity  trace operations
                 */
                explicit Alphabet(const OperatingMode how,
                                  const bool          verbosity=false) noexcept;

                //! cleanup
                virtual ~Alphabet() noexcept;

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

                //! emit byte to io, no model
                void write(StreamBits &io, const uint8_t byte);

                //! send EOS and fill io to byte boundary
                void flush(StreamBits &io);

                //! reset all alphabet
                void reset() noexcept;



            protected:
                const char *uid(const Unit &u) const noexcept;

                Unit       * const unit; //!< unit[0..Units-1]
                Emit               emit; //!< current prototype
                Unit       * const eos;  //!< End Of Stream unit
                Unit       * const nyt;  //!< Not Yet Transmitted unit
                Unit::List         used; //!< list of used units
                Frequency          sumf; //!< sum of frequencies
                
            public:
                const OperatingMode mode;       //!< operating mode
                bool                verbose;    //!< verbosity

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Alphabet);
                static const unsigned Required = Units * sizeof(Unit);
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
