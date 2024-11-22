//! file


#ifndef Y_Stream_Variable_Info_Included
#define Y_Stream_Varialbe_Info_Included 1


#include "y/config/starting.hpp"
#include "y/check/printf.hpp"
#include <cstdarg>

namespace Yttrium
{

    namespace IO
    {
        class VariableInfo_
        {

        public:
            virtual ~VariableInfo_() noexcept;

        protected:
            explicit VariableInfo_(char * const _info,
                                   const size_t _size) noexcept;

            // Methods
            void format(const char * const fmt, void * const ptr) noexcept;
            void assign(const VariableInfo_ &) noexcept;

        public:
            const char * operator()(const char * const fmt,...) noexcept Y_PRINTF_CHECK(2,3);


        private:
            char * const info;
            const size_t size;
            Y_DISABLE_COPY_AND_ASSIGN(VariableInfo_);
            void clear() noexcept;
        };

        template <const size_t SIZE>
        class VariableData
        {
        public:
            inline virtual ~VariableData() noexcept {}

        protected:
            inline explicit VariableData() noexcept : _info() {}

            char _info[SIZE];
        private:
            Y_DISABLE_COPY_AND_ASSIGN(VariableData);
        };

        template <const size_t SIZE>
        class VariableInfo : public VariableData<SIZE>, public VariableInfo_
        {
        public:
            typedef VariableData<SIZE> Data;
            typedef VariableInfo_      Info;
            using Data::_info;

            inline explicit VariableInfo() noexcept  : Data(), Info(_info,SIZE) {}
            inline virtual ~VariableInfo() noexcept {}

            inline VariableInfo(const VariableInfo &other) noexcept : Data(), Info(_info,SIZE)
            {
                assign(other);
            }

            inline VariableInfo & operator=(const VariableInfo &other)
            {
                assign(other);
                return *this;
            }

            inline explicit VariableInfo(const char * const fmt,...) noexcept Y_PRINTF_CHECK(2,3):
            Data(), Info(_info,SIZE)
            {
                assert(0!=fmt);
                va_list ap;
                va_start(ap,fmt);
                format(fmt,&ap);
                va_end(ap);
            }

            //! access to precomputed value
            inline const char * operator*() const noexcept { return _info; }


        };

    }

}

#endif

