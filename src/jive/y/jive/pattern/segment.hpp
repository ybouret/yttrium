
//! \file

#ifndef Y_Jive_Pattern_Included
#define Y_Jive_Pattern_Included 1

#include "y/jive/token.hpp"
#include "y/type/fourcc.hpp"

namespace Yttrium
{
    namespace Jive
    {


        class Segment : public Object
        {
        public:
            Segment       *next;
            Segment       *prev;
            void         * self;
            const uint32_t uuid;

            virtual bool contains(const uint8_t code) const noexcept = 0;


            virtual ~Segment() noexcept;

            static Segment * Optimize(Segment *seg);
            

        protected:
            explicit Segment(const uint32_t)  noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Segment);
        };

        class Single : public Segment
        {
        public:
            static const uint32_t UUID = Y_FOURCC('s', 'i', 'n', 'g');

            explicit Single(const uint8_t code) noexcept;
            virtual ~Single() noexcept;
            virtual bool contains(const uint8_t code) const noexcept;

            const uint8_t value;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Single);
        };

        class Couple : public Segment
        {
        public:
            static const uint32_t UUID = Y_FOURCC('d', 'u', 'a', 'l');
            explicit Couple(const uint8_t lo, const uint8_t hi) noexcept;
            virtual ~Couple() noexcept;

            virtual bool contains(const uint8_t code) const noexcept;

            const uint8_t lower;
            const uint8_t upper;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Couple);
        };

        class Segments
        {
        public:

        private:

        };

    }

}

#endif
