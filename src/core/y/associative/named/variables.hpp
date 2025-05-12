
//! \file

#ifndef Y_Named_Variables_Included
#define Y_Named_Variables_Included 1

#include "y/string.hpp"
#include "y/quantized.hpp"
#include "y/memory/out-of-reach.hpp"
#include "y/memory/buffer/zone.hpp"
#include "y/associative/suffix/set.hpp"
#include "y/type/nullify.hpp"

namespace Yttrium
{

    namespace Named
    {

        template <typename T>
        class Variable : public Quantized, public Counted, public Memory::ReadOnlyBuffer
        {
        public:
            Y_ARGS_EXPOSE(T,Type);

            template <typename NAME>
            inline explicit Variable(const NAME &n, ConstType u) : name(n), uuid(u) {}
            inline virtual ~Variable() noexcept {}

            inline friend std::ostream & operator<<(std::ostream &os, const Variable &v)
            {
                os << v.name << '=' << v.uuid;
                return os;
            }

            inline virtual const void *ro_addr() const noexcept { return Memory::OutOfReach::Addr(&uuid); }
            inline virtual size_t      measure() const noexcept { return sizeof(T); }

            const String name;
            ConstType    uuid;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Variable);
        };


        template <typename VARIABLE>
        class ByName : public ArcPtr<const VARIABLE>
        {
        public:
            typedef ArcPtr<const VARIABLE> Pointer;
            typedef String                 KeyType;

            inline explicit ByName(const VARIABLE * const _) noexcept : Pointer(_) {}
            inline virtual ~ByName() noexcept {}
            inline ByName(const ByName &_) noexcept : Pointer(_) {}

            inline const KeyType & key() const noexcept { return (**this).name; }

        private:
            Y_DISABLE_ASSIGN(ByName);
        };

        template <typename VARIABLE>
        class ByUUID: public ArcPtr<const VARIABLE>
        {
        public:
            typedef ArcPtr<const VARIABLE> Pointer;
            typedef Memory::ReadOnlyBuffer KeyType;

            inline explicit ByUUID(const ByName<VARIABLE> &_) noexcept : Pointer(_) {}
            inline virtual ~ByUUID() noexcept {}
            inline ByUUID(const ByUUID &_) noexcept : Pointer(_) {}

            inline const KeyType & key() const noexcept { return (**this); }

        private:
            Y_DISABLE_ASSIGN(ByUUID);
        };

        template < typename VARIABLE>
        class Variables
        {
        public:
            typedef typename VARIABLE::ConstType    ConstType;
            typedef ByName<VARIABLE>                VarByName;
            typedef ByUUID<VARIABLE>                VarByUUID;
            typedef typename VarByName::KeyType     KeyByName;
            typedef typename VarByUUID::KeyType     KeyByUUID;
            typedef SuffixSet<KeyByName,VarByName>  SetByName;
            typedef SuffixSet<KeyByUUID,VarByUUID>  SetByUUID;

            class Code : public Quantized
            {
            public:
                inline explicit Code() {}
                inline virtual ~Code() noexcept {}
                SetByName     byName;
                SetByUUID     byUUID;
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Code);
            };


            template <typename UNKNOWN>
            inline explicit Variables(const UNKNOWN &onUnknown, ConstType onMissing) :
            unknown(onUnknown),
            missing(onMissing),
            code( new Code() )
            {}

            inline virtual ~Variables() noexcept { assert(0!=code); Nullify(Coerce(code)); }

            const Code & operator*()  const noexcept { assert(0!=code); return *code; }
            const Code * operator->() const noexcept { assert(0!=code); return  code; }

            template <typename NAME> inline
            bool operator()(const NAME &name, ConstType uuid)
            {
                const VarByName   _( new VARIABLE(name,uuid) );
                const KeyByName & k = _->name;

                if(!code->byName.insert(_)) return false;
                try {
                    const VarByUUID __(_);
                    if(!code->byUUID.insert(__))
                    {
                        code->byName.remove(k);
                        return false;
                    }
                }
                catch(...) { code->byName.remove(k); throw; }
                return true;
            }

            inline ConstType uuid(const String &key) const noexcept
            {
                assert(0!=code);
                const VarByName * const var = code->byName.search(key);
                if(0!=var) return (*var)->uuid;
                return missing;
            }

            inline ConstType uuid(const char * const key) const
            {
                const String _(key); return uuid(_);
            }

            inline const String &name(ConstType byUUID) const noexcept
            {
                assert(0!=code);
                const Memory::Zone      key( &byUUID, sizeof(byUUID) );
                const VarByUUID * const var = code->byUUID.search( key );
                if(0!=var)  return (*var)->name;
                return unknown;
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Variables);
            const String unknown;
            ConstType    missing;
            Code * const code;
        };

    }

}

#endif

