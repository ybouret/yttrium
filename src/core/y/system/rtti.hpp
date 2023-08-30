//! \file

#ifndef Y_RTTI_Included
#define Y_RTTI_Included 1

#include "y/string.hpp"
#include <typeinfo>

namespace Yttrium
{
    class RTTI : public Object, public Counted
    {
    public:
        explicit RTTI(const String &);
        virtual ~RTTI() noexcept;
        Y_OSTREAM_PROTO(RTTI);

        static const RTTI   & Of(const std::type_info &);
        static const String & Name(const std::type_info &);

        static void Alias(const std::type_info &, const String &);
        static void Alias(const std::type_info &, const char   *);

        template <typename T> static inline
        const RTTI & Of() { return Of( typeid(T) ); }

        template <typename T> static inline
        const String & Name() { return Name( typeid(T) ); }

        template <typename T> static inline
        void Alias(const String &id) { Alias( typeid(T), id); }

        template <typename T> static inline
        void Alias(const char *id) { Alias( typeid(T), id); }


        const String & uuid() const noexcept;
        const String & name() const noexcept;
        bool           owns(const String &) const noexcept;
        void           aka(const String &); //!< NEW alias
        void           rem() noexcept;      //!< remove last

    private:
        Y_DISABLE_COPY_AND_ASSIGN(RTTI);
        class Code;
        Code *code;
    };
}
#endif

