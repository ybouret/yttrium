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
                typedef void (Alphabet:: *Emit)(StreamBits &io, const uint8_t); //!< emit prototype

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________

                //! setup
                /**
                 \param useEOS if true, assume EOS is used
                 */
                explicit Alphabet(const bool useEOS) noexcept;

                //! cleanup
                virtual ~Alphabet() noexcept;

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________
                void reset()                   noexcept;        //!< reset all
                void reduceFrequencies()       noexcept;        //!< reduce frequencies
                void display(std::ostream &os)    const;        //!< display status
                void write(StreamBits &io, const uint8_t byte); //!< emit and check sumf


                Unit::List   used; //!< current used units
                Emit         emit; //!< current emit proc
                Unit * const unit; //!< unit[0..Universe-1]
                Unit * const nyt;  //!< NYT unit
                Unit * const eos;  //!< EOS unit
                uint32_t     sumf; //!< cumulative sum of frequencies
                unsigned     nchr; //!< 0..256

                Unit  *      ctrl[Unit::Controls]; //!< involved control units [NYT[,EOS]]
                const size_t nctl;                 //!< 1 or 2...

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
