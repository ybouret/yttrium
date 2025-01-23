
//! \file

#ifndef Y_Apex_Jig_Included
#define Y_Apex_Jig_Included 1

#include "y/apex/block/jig/api.hpp"
#include "y/type/ints.hpp"
#include "y/calculus/base2.hpp"
#include "y/calculus/bit-count.hpp"
#include "y/text/hexadecimal.hpp"
#include <iostream>

namespace Yttrium
{
    namespace Apex
    {

        //! helper for sanity check
#define Y_APEX_JIG_ASSERT(EXPR) do { if( !(EXPR) ) { std::cerr << "Jig<" << PLAN << "> failure '" << #EXPR << "'" << std::endl; return false; } } while(false)

        //______________________________________________________________________
        //
        //
        //
        //! Jig based on Plan
        //
        //
        //______________________________________________________________________
        template <Plan PLAN>
        class Jig : public JigAPI
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const unsigned                         WordShift = PLAN;             //!< alias
            static const unsigned                         WordBytes = (1 << WordShift); //!< alias
            static const unsigned                         WordBits  = WordBytes << 3;   //!< alias
            typedef typename UnsignedInt<WordBytes>::Type Word;                         //!< alias
            typedef typename SignedInt<WordBytes>::Type   SignedWord;

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup from user's memory
            inline explicit Jig(void * const entry,
                                const size_t range) noexcept :
            JigAPI(range >> WordShift), word( static_cast<Word *>(entry) )
            {
                assert( IsPowerOfTwo(range) );
            }

            //! cleanup
            inline virtual ~Jig() noexcept {}

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! convert bytes to words
            static inline size_t BytesToWords(const size_t bytes) noexcept
            {
                return  (Y_ALIGN_TO(Word,bytes)) >> WordShift;
            }

            //__________________________________________________________________
            //
            //
            // Interface
            //
            //__________________________________________________________________

            //! Hexadecimal display
            virtual void display(std::ostream &os)  const  {
                os << '[';
                if(words<=0) {
                    const Word zero = 0;
                    os << Hexadecimal(zero);
                }
                else {
                    size_t i = words;
                    while(i-- > 0) {
                        os << Hexadecimal(word[i]);
                        if(i>0) os << ' ';
                    }
                }
                os << ']';
            }

            //! recompute words from bits
            virtual void setBits(const size_t bits) noexcept
            {
                Coerce(words) = BytesToWords( Y_Apex_Bytes_For(bits) );
            }

            //! upgrade using most significant word
            virtual size_t upgrade() noexcept
            {
                size_t &num = (Coerce(words) = count);
                size_t  msi = num-1;
                Word    msw = 0;

                while(num>0) {
                    msw = word[msi];
                    if(msw>0) break;
                    --num;
                    --msi;
                }

                return num <=0 ? 0 : msi * WordBits + BitCount::For(msw);
            }

            //! sanity check
            virtual bool chkBits(const size_t bits) const noexcept {

                Y_APEX_JIG_ASSERT(words<=count);

                for(size_t i=words+1;i<count;++i) {
                    Y_APEX_JIG_ASSERT(0==word[i]);
                }

                if(words>0) {
                    const size_t msi = words-1;
                    const Word   msw = word[msi];
                    Y_APEX_JIG_ASSERT(0!=msw);
                    Y_APEX_JIG_ASSERT(bits==WordBits*msi+BitCount::For(msw));
                }
                else
                {
                    Y_APEX_JIG_ASSERT(0==bits);
                }
                return true;
            }
            
            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            Word * const word; //!< user's memory

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Jig);
        };

    }

}

#endif

