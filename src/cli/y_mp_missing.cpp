#include "y/stream/proc/input.hpp"
#include "y/program.hpp"
#include "y/sequence/vector.hpp"
#include "y/string/tokenizer.hpp"
#include "y/jive/pattern/matcher.hpp"
#include "y/system/exception.hpp"
#include "y/container/algo/crop.hpp"
#include "y/associative/suffix/set.hpp"

using namespace Yttrium;


namespace
{

    typedef SuffixSet<String,String> PortDB;


    class Port : public Object
    {
    public:
        typedef CxxListOf<Port> List;

        explicit Port(const String  &_name,
                      Jive::Matcher &match,
                      size_t        &level,
                      PortDB        &missing) :
        name(_name),
        deps(),
        next(0),
        prev(0)
        {
            Core::Indent(std::cerr << " |",level<<1,'_') << "[" << name << "]" << std::endl;
            //std::cerr << "missing=" << missing << std::endl;
            Vector<String> fields(32,AsCapacity);;
            getAllDeps(fields,match);
            ++level;
            for(size_t i=fields.size();i>0;--i)
            {
                const String & portName = fields[i];
                if( IsInstalled(portName)    ) continue; // already installed
                if( missing.search(portName) ) continue; // already registered
                missing.insert(portName);
                deps.pushTail( new Port(portName,match,level,missing) );
            }
            --level;
        }

        virtual ~Port() noexcept
        {
        }

        const String name;
        List         deps;
        Port        *next;
        Port        *prev;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Port);

        inline
        void getAllDeps(Vector<String> &fields, Jive::Matcher &match)
        {
            const String   cmd = "port deps " + name;
            ProcInput      fp(cmd);
            String         line;

            while( fp.gets(line) )
            {
                if( line.size()<=0)             continue;
                if( !match.somehow(line,line) ) continue;

                {
                    const char * const data = strchr( line.c_str(), ':');
                    if(!data) throw Specific::Exception(name.c_str(),"corrupted missing ':'");
                    line = data+1;
                }
                Tokenizer::AppendTo(fields, line, ',');
            }
            for(size_t i=fields.size();i>0;--i)
                Algo::Crop(fields[i], isspace);
            //std::cerr << "deps: " << fields << std::endl;
        }

    public:
        static inline
        bool IsInstalled(const String &portName)
        {

            Vector<String> ans(8,AsCapacity);
            {
                const String   cmd = "port installed " + portName;
                ProcInput     fp(cmd);
                String        line;
                while( fp.gets(line) )
                {
                    // std::cerr << "[installed " << portName << "] '" << line << "'" << std::endl;
                    ans << line;
                }
            }
            //std::cerr << "[" << portName << "]=" << ans << std::endl;
            const size_t na = ans.size();
            switch(na)
            {
                case 0:
                case 1:
                    return false;

                case 2:
                    break;

                default:
                    throw Specific:: Exception( portName.c_str(), "unexpected installed answer");
            }
            String &id = ans[2];
            Algo::Crop(id,isspace);
            const char  * const org = id.c_str();
            char  * const       end = (char *)strchr(org,' ');
            if(!end) throw Specific:: Exception( portName.c_str(), "missing space after '%s'", org);
            *end = 0;
            //std::cerr << "org='" << org << "'" << std::endl;
            //if( 0 != memcmp(org,portName.c_str(),end-org)) throw Specific::Exception( portName.c_str(), "mismatch '%s", org);
            return true;
        }

    };
}

Y_Program()
{
    if(argc<=1)
    {
        std::cerr << "usage: " << program << " portname" << std::endl;
        return 0;
    }


    const String   name  = argv[1];
    if( Port::IsInstalled(name) ) return 0;
    PortDB         missing; missing.insert( name );
    Jive::Matcher  match = "Dependencies:";
    size_t         deep  = 1;
    AutoPtr<Port>  port  = new Port(name,match,deep,missing);


}
Y_End()

