
#include "y/jive/lexical/scanner.hpp"
#include "y/jive/pattern/first-chars.hpp"
#include "y/system/exception.hpp"
#include "y/associative/suffix/set.hpp"
#include "y/associative/hash/set.hpp"
#include "y/data/small/light/list/bare.hpp"
#include "y/type/nullify.hpp"

namespace Yttrium
{
    namespace Jive
    {
        namespace Lexical
        {

            namespace
            {


                typedef Small::BareLightList<const Action> ActionList;



                class Starting : public Object, public Counted, public ActionList
                {
                public:
                    typedef ArkPtr<uint8_t,Starting> Pointer;

                    class KeyDumper
                    {
                    public:
                        inline  KeyDumper() noexcept {}
                        inline ~KeyDumper() noexcept {}
                        size_t operator()(const uint8_t k) const noexcept { return k; }
                    private:
                        Y_DISABLE_COPY_AND_ASSIGN(KeyDumper);
                    };

                    typedef HashSet<uint8_t,Starting::Pointer,KeyDumper> Set;

                    inline explicit Starting(const uint8_t b) :
                    Object(),
                    Counted(),
                    ActionList(),
                    byte(b)
                    {
                    }

                    inline virtual ~Starting() noexcept {}

                    inline const uint8_t & key() const noexcept { return byte; }

                    const uint8_t byte;

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Starting);
                };
            }

            class Scanner:: Code : public Object
            {
            public:
                inline explicit Code(const String &id) :
                Object(),
                name(id),
                plan(),
                dict( new Dictionary() )
                {

                }

                inline virtual ~Code() noexcept {}

                inline void append(const Action::Pointer &which)
                {

                    assert(!which->motif->isFragile());

                    //----------------------------------------------------------
                    // try to insert into plan
                    //----------------------------------------------------------
                    if(!plan.insert(which))
                        throw Specific::Exception( name.c_str(), "multiple callback '%s'", which->name->c_str());

                    //----------------------------------------------------------
                    // query first chars
                    //----------------------------------------------------------
                    FirstChars fc;
                    which->motif->query(fc);

                    //----------------------------------------------------------
                    // dispatch action to all matching chars
                    //----------------------------------------------------------
                    for(unsigned i=0;i<256;++i)
                    {
                        const uint8_t byte(i);
                        if( !fc[byte] ) continue;
                        lookFor(byte) << *which;
                    }


                }

                const String &                    name;
                SuffixSet<String,Action::Pointer> plan;
                Starting::Set                     hub;
                ArcPtr<Dictionary>                dict;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Code);
                inline Starting &lookFor(const uint8_t byte)
                {
                    {
                        Starting::Pointer *pps = hub.search(byte);
                        if(pps) return (**pps);
                    }


                    Starting *starting = new Starting(byte);
                    {
                        const Starting::Pointer temp(starting);
                        if(!hub.insert(temp))
                            throw Specific::Exception(name(), "corrupted internal hub");
                    }
                    return *starting;
                }

            };

            Scanner::Code * Scanner:: Initialize(const String &id, Dictionary * &dict )
            {
                std::cerr << "sizeof(Scanner::Code)=" << sizeof(Code) << std::endl;
                std::cerr << "sizeof(Starting)     =" << sizeof(Starting) << std::endl;
                Code *c = new Code(id);
                dict = & *(c->dict);
                return c;
            }


            Scanner:: ~Scanner() noexcept
            {
                assert(0!=code);
                Nullify(code);
            }

            void Scanner:: append(const Action::Pointer &which)
            {
                assert(0!=code);
                code->append(which);
            }

            void Scanner:: noFragile(const Tag &id, const Motif &motif) const
            {
                if(motif->isFragile()) throw Specific::Exception(name->c_str(), "pattern '%s' if fragile", id->c_str());
            }
        }
    }

}

