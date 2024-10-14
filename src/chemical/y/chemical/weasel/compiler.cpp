
#include "y/chemical/weasel/compiler.hpp"
#include "y/chemical/weasel/parser.hpp"
#include "y/chemical/weasel/linker.hpp"

#include "y/system/exception.hpp"
#include "y/container/algo/crop.hpp"
#include "y/jive/pattern/matcher.hpp"

#include <cstring>

namespace Yttrium
{
    namespace Chemical
    {

        namespace Weasel
        {

            static const char * dbEqs[] =
            {
#include "db.hxx"
            };

            static const size_t dbNum = sizeof(dbEqs)/sizeof(dbEqs[0]);

            const char * const Compiler:: CallSign = "Weasel";

            namespace
            {
                typedef SuffixMap<String,String> EqsDB;
                typedef EqsDB::Iterator          Iterator;
            }

            class Compiler:: Code
            {
            public:

                inline  Code() :
                parser( CallSign ),
                linker( CallSign ),
                db()
                {
                    for(size_t i=0;i<dbNum;++i)
                    {
                        record(dbEqs[i]);
                    }
                }

                inline ~Code() noexcept {}

                inline void record(const char * const entry)
                {
                    assert(0!=entry);
                    const char * const sep = strchr(entry,':');
                    if(!sep) throw Specific::Exception(CallSign,"invalid database entry '%s'", entry);
                    String       name(entry,sep-entry); Algo::Crop(name,isspace);
                    const String data(entry);
                    if(!db.insert(name,data)) throw Specific::Exception(CallSign, "multiple database entry '%s'", name.c_str());
                }

                void compile(Library          &usrLib,
                             LuaEquilibria    &usrEqs,
                             Jive::Module     *input,
                             Sequence<String> *rxp)
                {
                    const AutoPtr<Jive::Syntax::XNode> ast =  parser.load(input);
                    linker(*ast,usrLib,usrEqs,rxp);
                }


                Parser         parser;
                Linker         linker;
                EqsDB          db;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Code);
            };



            static void *          _code[ Y_WORDS_FOR(Compiler::Code) ];
            static Compiler::Code * code = 0;


            void Compiler:: record(const String &entry)
            {
                assert(0!=code);
                code->record(entry.c_str());
            }

            void Compiler:: record(const char * const entry)
            {
                assert(0!=code);
                const String _(entry);
                record(_);
            }



            Compiler:: Compiler()
            {
                code = new ( Y_STATIC_ZARR(_code) ) Code();
            }

            Compiler:: ~Compiler() noexcept
            {
                assert(0!=code);
                Destruct(code);
                code = 0;
                Y_STATIC_ZARR(_code);
            }

            void Compiler:: operator()(Library       &usrLib,
                                       LuaEquilibria &usrEqs,
                                       Jive::Module  *input)
            {
                // first pass: compile and collect rx
                Vector<String> rx;
                code->compile(usrLib,usrEqs,input,&rx);


                // second pass: compile rx


                while(rx.size()>0)
                {
                    const String  regexp = rx.pullHead();
                    Jive::Matcher match  = regexp;
                    //std::cerr << "Looking for '" << regexp << "'" << std::endl;
                    size_t count = 0;
                    for(Iterator it=code->db.begin();it!=code->db.end();++it)
                    {
                        const String &name = it->key;
                        if( match.exactly(name,name) )
                        {
                            ++count;
                            code->compile(usrLib, usrEqs, Jive::Module::OpenData(name, *it), 0);
                        }
                    }
                    if(count<=0) throw Specific::Exception(CallSign, "no match for '%s' in database", regexp.c_str());
                }

            }

            void Compiler:: showDB() const
            {
                assert(0!=code);
                std::cerr << "<RecordedEquilibria>" << std::endl;
                for(Iterator it=code->db.begin();it!=code->db.end();++it)
                {
                    //const String &name = it->key;
                    std::cerr << "\t" << *it << std::endl;
                }
                std::cerr << "<RecordedEquilibria>" << std::endl;
            }

        }
    }

}

