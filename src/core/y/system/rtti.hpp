//! \file

#ifndef Y_RTTI_Included
#define Y_RTTI_Included 1

#include "y/string.hpp"
#include <typeinfo>

namespace Yttrium
{
    //__________________________________________________________________________
    //
    //
    //
    //! Run Time Type Information
    //
    //
    //__________________________________________________________________________
    class RTTI : public Object, public Counted
    {
    public:
        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
        explicit RTTI(const String &); //!< initial setup (uuid)
        virtual ~RTTI() noexcept;      //!< cleanup
        Y_OSTREAM_PROTO(RTTI);         //!< display

        //______________________________________________________________________
        //
        //
        // Static Methods
        //
        //______________________________________________________________________
        static const RTTI   & Of(const std::type_info &);   //!< get/create on the fly
        static const String & Name(const std::type_info &); //!< get name()

        static void Alias(const std::type_info &, const String &); //!< generate new alias
        static void Alias(const std::type_info &, const char   *); //!< generate new alias

        //! generic helper
        template <typename T> static inline
        const RTTI & Of() { return Of( typeid(T) ); }

        //! generic helper
        template <typename T> static inline
        const String & Name() { return Name( typeid(T) ); }

        //! generic helper
        template <typename T> static inline
        void Alias(const String &id) { Alias( typeid(T), id); }

        //! generi helper
        template <typename T> static inline
        void Alias(const char *id) { Alias( typeid(T), id); }

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        const String & uuid() const noexcept;               //!< system name
        const String & name() const noexcept;               //!< user's name
        bool           owns(const String &) const noexcept; //!< check within aliases
        void           aka(const String &);                 //!< NEW alias
        void           rem() noexcept;                      //!< remove last
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(RTTI);
        class Code;
        Code *code;
    };

    //! shortcut
    template <typename T> inline
    const String & TypeName()  { return RTTI::Name<T>(); }

}
#endif

