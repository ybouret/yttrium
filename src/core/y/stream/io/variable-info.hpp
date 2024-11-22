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

        //______________________________________________________________________
        //
        //
        //
        //! Operations on Variable Info
        //
        //
        //______________________________________________________________________
        class _VariableInfo
        {
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
        public:
            virtual ~_VariableInfo() noexcept; //!< cleanup

        protected:
            //! setup from user data
            explicit _VariableInfo(char * const _info,
                                   const size_t _size) noexcept;

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void format(const char * const fmt, void * const ptr) noexcept; //!< format using C_Format::Buffer
            void assign(const _VariableInfo &) noexcept;                    //!< assign (with truncation) other

        public:
            //! reformat
            const char * operator()(const char * const fmt,...) noexcept Y_PRINTF_CHECK(2,3);


        private:
            char * const info;
            const size_t size;
            Y_DISABLE_COPY_AND_ASSIGN(_VariableInfo);
            void clear() noexcept;
        };

        //______________________________________________________________________
        //
        //
        //
        //! Variable Info local Storage
        //
        //
        //______________________________________________________________________
        template <const size_t SIZE>
        class _VariableData
        {
            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
        public:
            inline virtual ~_VariableData() noexcept {}

        protected:
            inline explicit _VariableData() noexcept : _info() {}

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            char _info[SIZE]; //!< effective data
        private:
            Y_DISABLE_COPY_AND_ASSIGN(_VariableData);
        };

        //______________________________________________________________________
        //
        //
        //
        //! Variable Info 
        //
        //
        //______________________________________________________________________
        template <const size_t SIZE>
        class VariableInfo : public _VariableData<SIZE>, public _VariableInfo
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            typedef _VariableData<SIZE> Data; //!< alias
            typedef _VariableInfo       Info; //!< alias
            using Data::_info;

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________

            //! setup emtpy
            inline explicit VariableInfo() noexcept  : Data(), Info(_info,SIZE) {}

            //! clenaup
            inline virtual ~VariableInfo() noexcept {}

            //! copy
            inline VariableInfo(const VariableInfo &other) noexcept : Data(), Info(_info,SIZE)
            {
                assign(other);
            }

            //! assign
            inline VariableInfo & operator=(const VariableInfo &other)
            {
                assign(other);
                return *this;
            }

            

        };

    }

}

#endif

