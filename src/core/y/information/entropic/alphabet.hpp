//! \file

#ifndef Y_Information_Entropic_Alphabet_Included
#define Y_Information_Entropic_Alphabet_Included 1

#include "y/information/entropic/model.hpp"
#include "y/calculus/align.hpp"
#include "y/data/list/raw.hpp"
#include "y/stream/bits.hpp"

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
                typedef void (Alphabet::*Emit)(StreamBits &, Unit &);   //!< emit prototype

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

                //! emit byte to io and build model
                void write(StreamBits &io, const uint8_t byte, Model &model);


                //! send EOS and fill io to byte boundary
                void flush(StreamBits &io);

                //! reset all alphabet
                void reset() noexcept;

                //! reset with a different mode
                void reset(const OperatingMode how) noexcept;


                //! reduce frequencies
                void reduce() noexcept;

                void display(std::ostream &os) const;


            protected:
                Unit       * const unit; //!< unit[0..Units-1]
                Emit               emit; //!< current prototype
                Unit       * const nyt;  //!< Not Yet Transmitted unit
                Unit       * const eos;  //!< End Of Stream unit

            public:
                Unit::List         used; //!< list of used units
                Frequency          sumf; //!< sum of frequencies
            public:
                const OperatingMode mode;       //!< operating mode
                const size_t        full;       //!< switching to emitFull
                bool                verbose;    //!< verbosity

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Alphabet);
                static const unsigned Required = Unit::Universe * sizeof(Unit);
                void *wksp[ Y_WORDS_GEQ(Required) ];

                void init() noexcept; //!< set all units
                void send(StreamBits &io, const Unit &u); //!< send unit to io
                void rank(Unit &u)              noexcept; //!< keep ranked used
                void pushControls()             noexcept; //!< push initial controls
                void emitInit(StreamBits &io, Unit &u);   //!< emit first unit
                void emitBulk(StreamBits &io, Unit &u);   //!< emit bulk unit
                void emitFull(StreamBits &io, Unit &u);   //!< emit unit with full Alphabet
                void write_(StreamBits &io, const uint8_t byte); //!< write and check frequencies

            };

            

        }

    }

}

#endif
