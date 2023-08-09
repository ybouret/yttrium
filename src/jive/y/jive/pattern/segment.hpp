//! \file

#ifndef Y_Jive_Pattern_Segment_Included
#define Y_Jive_Pattern_Segment_Included 1

#include "y/jive/token.hpp"
#include "y/data/list/cxx.hpp"

namespace Yttrium
{
    namespace Jive
    {

        
        class Segment : public Object
        {
        public:
            typedef CxxListOf<Segment> List;
            explicit Segment(const uint8_t a, const uint8_t b) noexcept;
            explicit Segment(const uint8_t a) noexcept;

            virtual ~Segment() noexcept;
            Y_OSTREAM_PROTO(Segment);
            
            bool contains(const uint8_t) noexcept;

            Segment *     next;
            Segment *     prev;
            const uint8_t lower;
            const uint8_t upper;

            inline const Segment & operator*() const noexcept { return *this; }

            static void AppendTo(List &, Segment *s) noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Segment);
        };

    }

}

#endif
