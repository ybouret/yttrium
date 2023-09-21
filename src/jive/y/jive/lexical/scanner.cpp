
#include "y/jive/lexical/scanner.hpp"
#include "y/system/exception.hpp"
#include "y/associative/suffix/set.hpp"
#include "y/type/nullify.hpp"

namespace Yttrium
{
    namespace Jive
    {
        namespace Lexical
        {
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

                inline void append(const Deed &deed)
                {
                    if(!plan.insert(deed))
                        throw Specific::Exception( name.c_str(), "multiple callback '%s'", deed->name->c_str());
                }

                const String          &name;
                SuffixSet<String,Deed> plan;
                ArcPtr<Dictionary>     dict;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Code);
            };

            Scanner::Code * Scanner:: Initialize(const String &id, Dictionary * &dict )
            {
                std::cerr << "sizeof(Scanner::Code)=" << sizeof(Code) << std::endl;
                Code *c = new Code(id);
                dict = & *(c->dict);
                return c;
            }


            Scanner:: ~Scanner() noexcept
            {
                assert(0!=code);
                Nullify(code);
            }

            void Scanner:: append(const Deed &deed)
            {
                assert(0!=code);
                code->append(deed);
            }
        }
    }

}

