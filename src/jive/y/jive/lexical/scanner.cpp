
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

                //______________________________________________________________
                //
                //
                //! list of action reference
                //
                //______________________________________________________________
                typedef Small::BareLightList<Action> ActionList;


                //______________________________________________________________
                //
                //
                //! list of actions for a given starting byte
                //
                //______________________________________________________________
                class Starting : public Object, public Counted, public ActionList
                {
                public:
                    //__________________________________________________________
                    //
                    //
                    // Definitions
                    //
                    //__________________________________________________________
                    typedef ArkPtr<uint8_t,Starting> Pointer; //!< alias

                    //! specific key hasher
                    class KeyDumper
                    {
                    public:
                        inline  KeyDumper() noexcept {}
                        inline ~KeyDumper() noexcept {}
                        size_t operator()(const uint8_t k) const noexcept { return k; }
                    private:
                        Y_DISABLE_COPY_AND_ASSIGN(KeyDumper);
                    };

                    //! making a hash set from byte, pointer and key dumper
                    typedef HashSet<uint8_t,Starting::Pointer,KeyDumper> Set;


                    //__________________________________________________________
                    //
                    //
                    // C++
                    //
                    //__________________________________________________________
                   
                    //! initialize empty
                    inline explicit Starting(const uint8_t b) noexcept:
                    Object(),
                    Counted(),
                    ActionList(),
                    byte(b)
                    {
                    }

                    //! cleanup
                    inline virtual ~Starting() noexcept {}

                    //__________________________________________________________
                    //
                    //
                    // Methods
                    //
                    //__________________________________________________________

                    //! key for API
                    inline const uint8_t & key() const noexcept { return byte; }

                    //__________________________________________________________
                    //
                    //
                    // Members
                    //
                    //__________________________________________________________
                    const uint8_t byte; //!< first common char

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Starting);
                };

                typedef SuffixSet<String,Action::Pointer> Plan;

            }


            //__________________________________________________________________
            //
            //
            //
            //! Data and Algorithm for Lexical::Scanner
            //
            //
            //__________________________________________________________________
            class Scanner:: Code : public Object, public Plan
            {
            public:
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                inline explicit Code(const String &id) :
                Object(),
                Plan(),
                name(id),
                hub(),
                dict( new Dictionary() )
                {

                }

                inline virtual ~Code() noexcept {}

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

                //! add a new action
                inline void add(Action::Pointer &which)
                {

                    assert(!which->motif->isFragile());

                    //----------------------------------------------------------
                    // try to insert into plan
                    //----------------------------------------------------------
                    if(!insert(which))
                        throw Specific::Exception( name.c_str(), "multiple callback '%s'", which->name->c_str());

                    try 
                    {

                        //------------------------------------------------------
                        // query first chars
                        //------------------------------------------------------
                        FirstChars fc;
                        which->motif->query(fc);

                        //------------------------------------------------------
                        // dispatch action to all matching first chars
                        //------------------------------------------------------
                        for(unsigned i=0;i<256;++i)
                        {
                            const uint8_t byte(i);
                            if( !fc[byte] ) continue;
                            lookFor(byte) << *which;
                        }
                    }
                    catch(...)
                    {
                        remove(which->key());
                        throw;
                    }


                }

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                const String &                    name; //!< Scanner's name
                Starting::Set                     hub;  //!< map first char to action
                ArcPtr<Dictionary>                dict; //!< shared dictionary

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
                Coerce(dict) = 0;
                Nullify(code);
            }

            void Scanner:: submitCode(Action::Pointer &which)
            {
                assert(0!=code);
                if(which->motif->isFragile())
                    throw Specific::Exception(name->c_str(), "pattern '%s' if fragile", which->name->c_str());
                code->add(which);
            }

            void Scanner:: cleanup() noexcept
            {
                assert(0!=code);
                for(Code::Iterator it=code->begin();it!=code->end();++it)
                {
                    Action &a = **it;
                    std::cerr << "Cleaning " << a.name << std::endl;
                    a.motif->reset();
                }
            }


        }
    }

}

