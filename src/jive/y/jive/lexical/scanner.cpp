
#include "y/jive/lexical/scanner.hpp"
#include "y/jive/pattern/first-chars.hpp"
#include "y/jive/source.hpp"

#include "y/system/exception.hpp"
#include "y/associative/suffix/set.hpp"
#include "y/associative/hash/set.hpp"
#include "y/data/small/light/list/bare.hpp"
#include "y/type/nullify.hpp"
#include "y/text/ascii/embedding.hpp"
#include "y/text/ascii/printable.hpp"

#include "y/stream/xmlog.hpp"

namespace Yttrium
{
    namespace Jive
    {
        namespace Lexical
        {

            bool Scanner::Verbose = false;

            namespace
            {

                //______________________________________________________________
                //
                //
                //! list of action reference
                //
                //______________________________________________________________
                typedef Small::BareLightList<Action> ActionList;
                typedef ActionList::NodeType         ActionNode;

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

                //! standalone code
                inline explicit Code(const String &id) :
                Object(),
                Plan(),
                name(id),
                hub(),
                dict( new Dictionary() ),
                xml(  new XMLog(Scanner::Verbose) )
                {
                }

                //! standalone code
                inline explicit Code(const String &id, const Code &from) :
                Object(),
                Plan(),
                name(id),
                hub(),
                dict( from.dict ),
                xml(  from.xml  )
                {
                }



                //! cleanup
                inline virtual ~Code() noexcept {}

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________

                //______________________________________________________________
                //
                //! add a new action
                //______________________________________________________________
                inline void add(Action::Pointer &which)
                {
                    assert(!which->motif->isFragile());

                    //----------------------------------------------------------
                    // try to insert into plan
                    //----------------------------------------------------------
                    if(!insert(which))
                        throw Specific::Exception( name.c_str(), "multiple callback '%s'", which->name->c_str());

                    //----------------------------------------------------------
                    // make hub
                    //----------------------------------------------------------
                    try
                    {

                        //------------------------------------------------------
                        // query first chars
                        //------------------------------------------------------
                        FirstChars fc;
                        which->motif->query(fc);
                        Y_XMLOG(*xml,"<" << name << "> --> '" << which->name << "' to " << fc);


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
                //! find next action
                //______________________________________________________________
                inline ReturnValue  probe(Source &source, Action * &primary)
                {
                    assert(0==primary);
                    //----------------------------------------------------------
                    //
                    // test if still data in source
                    //
                    //----------------------------------------------------------
                    if(!source.ready())
                    {
                        Y_XMLOG(*xml,"<" << name << "> => EOS");
                        return AtEndOfStream;
                    }
                    assert(source.cached()>0);

                    //----------------------------------------------------------
                    //
                    // select motifs to test using pre-computed hub
                    //
                    //----------------------------------------------------------
                    const uint8_t      byte = source.peek();
                    Starting::Pointer *hook = hub.search(byte);
                    if(!hook)
                    {
                        Y_XMLOG(*xml,"no motif with first char '" << ASCII::Printable::Char[byte] << "'");
                        return ReturnFailure; // syntax error, no possible match
                    }

                    //----------------------------------------------------------
                    //
                    // find first matching node
                    //
                    //----------------------------------------------------------
                    ActionNode *node = (**hook).head; assert(0!=node);

                PROBE_FIRST:
                    if( (**node).motif->takes(source) )
                    {
                        Y_XMLOG(*xml,"<" << name << "> (+) primary : '" << (**node).name << "'='" << (**node).motif->toPrintable() << "'");
                        goto FOUND_FIRST;
                    }

                    if( 0 == (node=node->next) )
                    {
                        Y_XMLOG(*xml,"no first char matching '" << ASCII::Printable::Char[byte] << "'");
                        return  ReturnFailure; // syntax error, no possible match
                    }
                    goto PROBE_FIRST;

                FOUND_FIRST:
                    assert(0!=node);
                    primary = & **node;
                    size_t length = primary->motif->size;
                    assert(primary->motif->size>0);

                    //----------------------------------------------------------
                    //
                    // find a better match ?
                    //
                    //----------------------------------------------------------
                    for(node=node->next;node;node=node->next)
                    {
                        Action *replica = & **node;

                        //------------------------------------------------------
                        // restore source state
                        //------------------------------------------------------
                        source.dup( *(primary->motif) ); assert(source.cached()>=length);

                        if(replica->motif->takes(source) )
                        {
                            const size_t len = replica->motif->size;
                            if(len>length)
                            {
                                //----------------------------------------------
                                // new primary!
                                //----------------------------------------------
                                Y_XMLOG(*xml,"<" << name << "> (+) replica : '" << replica->name << "'='" << replica->motif->toPrintable() << "'");
                                primary->motif->release();
                                primary = replica;
                                length  = len;
                            }
                            else
                            {
                                //----------------------------------------------
                                // too late: retrieve source state
                                //----------------------------------------------
                                Y_XMLOG(*xml,"<" << name << "> (-) replica : '" << replica->name << "'='" << replica->motif->toPrintable() << "'");
                                source.put(*(replica->motif));
                                source.skip(length);
                            }
                        }
                        else
                        {
                            //--------------------------------------------------
                            // retrieve source state
                            //--------------------------------------------------
                            Y_XMLOG(*xml,"<" << name << "> (0) replica : '" << replica->name << "'");
                            assert(0==replica->motif->size);
                            source.skip(length);
                        }
                    }

                    
                    return  ReturnSuccess;

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
                ArcPtr<XMLog>                     xml;  //!< shared output
                
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

        }
    }

}

#include "y/type/temporary.hpp"

namespace Yttrium
{
    namespace Jive
    {
        namespace Lexical
        {

            XMLog & Scanner:: getXMLog() noexcept
            {
                assert(0!=code);
                return *(code->xml);
            }


            Scanner::Code * Scanner:: Initialize(const String &id, Dictionary * &dict)
            {
                Code *c = new Code(id);
                dict = & *(c->dict);
                return c;
            }

            Scanner::Code * Scanner:: Initialize(const String &id, Dictionary *&dict, const Scanner &parent)
            {
                assert(0!=parent.code);
                Code *c = new Code(id,*parent.code);
                dict = & *(c->dict);
                return c;
            }


            Scanner:: ~Scanner() noexcept
            {
                assert(0!=code);
                Coerce(dict) = 0;
                Nullify(code);
            }

            const Action & Scanner:: submitCode(Action::Pointer &which)
            {
                assert(0!=code);
                if(which->motif->isFragile())
                    throw Specific::Exception(name->c_str(), "pattern '%s' if fragile", which->name->c_str());
                code->add(which);
                return *which;
            }

            void Scanner:: cleanup() noexcept
            {
                assert(0!=code);
                Y_XML_SECTION_OPT( *(code->xml), *name, " cleaning='" << code->size() << "'");
                for(Code::Iterator it=code->begin();it!=code->end();++it)
                {
                    Action &a = **it;
                    Y_XMLOG(*(code->xml), "-- cleaning '" << a.name << "'");
                    a.motif->reset();
                }
            }


            bool Scanner:: isClean() const noexcept
            {
                assert(0!=code);
                bool flag = true;
                for(Code::Iterator it=code->begin();it!=code->end();++it)
                {
                    const Action &a = **it;
                    if(a.motif->size>0)
                    {
                        flag = false;
                        Y_XMLOG(*(code->xml), "-- not clean '" << a.name << "'");
                    }
                }
                return flag;
            }

            ReturnValue Scanner:: probe(Source &source, Action * &action)
            {
                assert(0!=code);
                assert(0==action);
                return code->probe(source,action);
            }

            Message Scanner:: produce(const Token &tkn)
            {
                assert(0!=code);
                Y_XMLOG(*(code->xml),"<" << name << "> @EMIT '" << tkn.toPrintable() << "'");
                return LX_EMIT;
            }

            Message Scanner:: newLineAndEmit(const Token &tkn)
            {
                assert(0!=code);
                Y_XMLOG(*(code->xml),"<" << name << "> @ENDL");
                return produce(tkn) | LX_ENDL;
            }

            Message Scanner:: newLineAndDrop(const Token &tkn)
            {
                return discard(tkn) | LX_ENDL;
            }

            Message Scanner:: discard(const Token &tkn)
            {
                Y_XMLOG(*(code->xml),"<" << name << "> @DROP '" << tkn.toPrintable() << "'");
                return LX_DROP;
            }

        }
    }

}

