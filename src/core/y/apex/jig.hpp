
//! \file

#ifndef Y_Apex_Jig_Included
#define Y_Apex_Jig_Included 1

#include "y/apex/jig/api.hpp"
#include "y/type/ints.hpp"
#include "y/calculus/base2.hpp"
#include "y/calculus/bit-count.hpp"
#include "y/text/hexadecimal.hpp"
#include <iostream>

namespace Yttrium
{
    namespace Apex
    {

#define Y_APEX_JIG_ASSERT(EXPR) do { if( !(EXPR) ) { std::cerr << "Jig<" << PLAN << "> failure '" << #EXPR << "'" << std::endl; return false; } } while(false)

        template <Plan PLAN>
        class Jig : public JigAPI
        {
        public:
            static const unsigned                         WordShift = PLAN;
            static const unsigned                         WordBytes = (1 << WordShift);
            static const unsigned                         WordBits  = WordBytes << 3;
            typedef typename UnsignedInt<WordBytes>::Type Word;

            inline explicit Jig(void * const entry,
                                const size_t range) noexcept :
            JigAPI(range >> WordShift), word( static_cast<Word *>(entry) )
            {
                assert( IsPowerOfTwo(range) );
            }

            inline virtual ~Jig() noexcept {}

            static inline size_t BytesToWords(const size_t bytes) noexcept
            {
                return  (Y_ALIGN_TO(Word,bytes)) >> WordShift;
            }

            virtual void display(std::ostream &os)  const  {
                os << '[';
                if(words<=0)
                {
                    const Word zero = 0;
                    os << Hexadecimal(zero);
                }
                else
                {
                    size_t i = words;
                    while(i-- > 0)
                    {
                        os << Hexadecimal(word[i]);
                        if(i>0) os << ' ';
                    }
                }
                os << ']';
            }

            virtual void setBits(const size_t bits) noexcept
            {
                Coerce(words) = BytesToWords( Y_Apex_Bytes_For(bits) );
            }

            virtual size_t upgrade() noexcept
            {
                size_t &num = (Coerce(words) = count);
                size_t  msi = num-1;
                Word    msw = 0;

                while(num>0)
                {
                    msw = word[msi];
                    if(msw>0) break;
                    --num;
                    --msi;
                }

                if(num<=0)
                {
                    return 0;
                }
                else
                {
                    return msi * WordBits + BitCount::For(msw);
                }
            }

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


            Word * const word;
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Jig);
        };

    }

}

#endif

