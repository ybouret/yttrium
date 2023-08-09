
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
            virtual void display(std::ostream &)      const          = 0;


            static Segment * Optimize(Segment *seg);
            static Segment * TryMerge(Segment *lhs, Segment *rhs);

            template <typename T>
            T *as() noexcept {
                assert(0!=self);
                assert(T::UUID==uuid);
                return static_cast<T*>(self);
            }

            Y_OSTREAM_PROTO(Segment);
            virtual ~Segment() noexcept;

        protected:
            explicit Segment(const uint32_t)  noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Segment);
        };


        class Ponctual : public Segment
        {
        public:
            static const uint32_t UUID = Y_FOURCC('_', 'o', 'n', 'e');

            explicit Ponctual(const uint8_t code) noexcept;
            virtual ~Ponctual() noexcept;
            virtual bool contains(const uint8_t) const noexcept;
            virtual void display(std::ostream &)          const;

            const uint8_t value;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Ponctual);
        };

        class Interval : public Segment
        {
        public:
            static const uint32_t UUID = Y_FOURCC('d', 'u', 'a', 'l');
            explicit Interval(const uint8_t lo, const uint8_t hi) noexcept;
            virtual ~Interval() noexcept;

            virtual bool contains(const uint8_t) const noexcept;
            virtual void display(std::ostream &)          const;

            const uint8_t lower;
            const uint8_t upper;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Interval);
        };

        class Segments
        {
        public:

        private:

        };

    }

}

#endif
