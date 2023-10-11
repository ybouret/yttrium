#include "y/system/rtti.hpp"
#include "y/data/list/cxx.hpp"
#include "y/associative/little-endian-address.hpp"
#include "y/singleton.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{


    class RTTI:: Code : public Object
    {
    public:
        class Alias : public String
        {
        public:
            typedef CxxListOf<Alias> List;

            template <typename NAME>
            inline explicit Alias(NAME &name) :
            String(name), next(0), prev(0)
            {
            }

            inline virtual ~Alias() noexcept {}

            Alias *next;
            Alias *prev;


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Alias);
        };


        explicit Code(const String &tid) : Object(),
        aliases()
        {
            aliases.pushTail( new Alias(tid) );
        }


        virtual ~Code() noexcept
        {
        }

        inline void aka(const String &id)
        {
            for(const Alias *alias=aliases.head;alias;alias=alias->next)
            {
                if(*alias==id)
                    throw Specific::Exception("RTTI","multiple alias '%s' for '%s'", id.c_str(), aliases.tail->c_str());
            }
            aliases.pushTail( new Alias(id) );
        }

        Alias::List aliases;


    private:
        Y_DISABLE_COPY_AND_ASSIGN(Code);
    };

    RTTI::RTTI(const String &tid) : code( new Code(tid) )
    {
    }

    RTTI:: ~RTTI() noexcept
    {
        assert(0!=code);
        delete code;
        code = 0;
    }

    const String & RTTI:: uuid() const noexcept
    {
        assert(0!=code);
        assert(code->aliases.size>0);
        return *(code->aliases.head);
    }

    const String & RTTI:: name() const noexcept
    {
        assert(0!=code);
        assert(code->aliases.size>0);
        return *(code->aliases.tail);
    }

    void RTTI:: aka(const String &id)
    {
        assert(0!=code);
        code->aka(id);
    }

    void RTTI:: rem() noexcept
    {
        assert(code!=0);
        if(code->aliases.size>1) delete code->aliases.popTail();
    }

    bool RTTI:: owns(const String &id) const noexcept
    {
        assert(code!=0);
        for(Code::Alias *alias=code->aliases.head;alias;alias=alias->next)
        {
            if(*alias == id) return true;
        }
        return false;
    }


    std::ostream & operator<<(std::ostream &os, const RTTI &rtti)
    {
        assert(0!=rtti.code);
        os << '[';
        for(const RTTI::Code::Alias *alias = rtti.code->aliases.head;alias;alias=alias->next)
        {
            os << ' ' << '\'' << *alias << '\'';
        }
        os << ' ' << ']';
        return os;
    }


}

#include "y/ptr/arc.hpp"
#include "y/associative/hash/map.hpp"

namespace Yttrium
{
    class RTTIdb : public Singleton<RTTIdb>
    {
    public:
        static const char * const      CallSign;
        static const AtExit::Longevity LifeTime = AtExit::MaximumLongevity - 11;

        typedef ArcPtr<RTTI>            RTTIptr;
        typedef HashMap<String,RTTIptr> RTTImap;

        RTTI & getPrimary(const std::type_info &ti)
        {
            const String key = ti.name();
            RTTIptr     *ppr = db.search(key);
            if(0!=ppr)
            {
                return **ppr;
            }
            else
            {
                RTTI *pr = new RTTI(key);
                const RTTIptr tmp(pr);
                if(!db.insert(key,tmp))
                    throw Specific::Exception(CallSign,"unexpected failure to insert primary '%s'", key.c_str());
                return *pr;
            }
        }


        inline RTTI & update(const std::type_info &ti,
                             const String         &id)
        {
            RTTI        &primary = getPrimary(ti);
            if(primary.owns(id))
            {
                assert(db.search(id));
                return primary;
            }
            
            //------------------------------------------------------------------
            //
            // second lookup
            //
            //------------------------------------------------------------------
            RTTIptr *pps = db.search(id);
            if(pps)
                throw Specific::Exception(CallSign,"alias '%s' already for '%s'", id.c_str(), (**pps).name().c_str());

            //------------------------------------------------------------------
            //
            // update aliases, shoudln't fail
            //
            //------------------------------------------------------------------
            primary.aka(id);

            //------------------------------------------------------------------
            //
            // register replica
            //
            //------------------------------------------------------------------
            const RTTIptr replica( &primary );
            try
            {
                if(!db.insert(id,replica))
                    throw Specific::Exception(CallSign,"unexpected failure to insert '%s' for '%s'", id.c_str(), primary.name().c_str());
            }
            catch(...)
            {
                primary.rem(); // emergency exit
                throw;
            }

            return primary;

        }




    private:
        Y_DISABLE_COPY_AND_ASSIGN(RTTIdb);
        friend class Singleton<RTTIdb>;
        explicit        RTTIdb();
        inline virtual ~RTTIdb() noexcept {}

        RTTImap db;


    };

    const char * const RTTIdb:: CallSign = "RTTI";

}

namespace Yttrium
{
    const RTTI & RTTI:: Of(const std::type_info &ti)
    {
        static RTTIdb &rdb = RTTIdb::Instance();
        return rdb.getPrimary(ti);
    }

    const String & RTTI:: Name(const std::type_info &ti)
    {
        static RTTIdb &rdb = RTTIdb::Instance();
        return rdb.getPrimary(ti).name();
    }

    void RTTI:: Alias(const std::type_info &ti, const String &tid)
    {
        static RTTIdb &rdb = RTTIdb::Instance();
        rdb.update(ti,tid);
    }

    void RTTI:: Alias(const std::type_info &ti, const char  *tid)
    {
        const String _(tid);
        Alias(ti,_);
    }
}

#include "y/apex/rational.hpp"
#include "y/mkl/xreal.hpp"
#include "y/mkl/complex.hpp"

namespace Yttrium
{

#define Y_RTTI(NAME) do {       \
update(typeid(NAME),#NAME);     \
} while(false)


    RTTIdb:: RTTIdb() : Singleton<RTTIdb>(), db()
    {
#if 0
        Y_RTTI(char);
        Y_RTTI(short);
        Y_RTTI(int);
        Y_RTTI(long);
        Y_RTTI(long int);
        Y_RTTI(long long int);
        Y_RTTI(long long);

        Y_RTTI(unsigned char);
        Y_RTTI(unsigned short);
        Y_RTTI(unsigned int);
        Y_RTTI(unsigned long int);
        Y_RTTI(unsigned long long int);
        Y_RTTI(unsigned);
        Y_RTTI(unsigned long);
        Y_RTTI(unsigned long long);

        Y_RTTI(uint8_t);
        Y_RTTI(uint16_t);
        Y_RTTI(uint32_t);
        Y_RTTI(uint64_t);

        Y_RTTI(int8_t);
        Y_RTTI(int16_t);
        Y_RTTI(int32_t);
        Y_RTTI(int64_t);

        Y_RTTI(ptrdiff_t);
        Y_RTTI(size_t);
#endif

        Y_RTTI(float);
        Y_RTTI(double);
        Y_RTTI(long double);
        Y_RTTI(String);

        Y_RTTI(apn);
        Y_RTTI(apz);
        Y_RTTI(apq);

        Y_RTTI(XReal<float>);
        Y_RTTI(XReal<double>);
        Y_RTTI(XReal<long double>);

        Y_RTTI(Complex<float>);
        Y_RTTI(Complex<double>);
        Y_RTTI(Complex<long double>);

        Y_RTTI(Complex< XReal<float> >);
        Y_RTTI(Complex< XReal<double> >);
        Y_RTTI(Complex< XReal<long double> >);

    }
}
