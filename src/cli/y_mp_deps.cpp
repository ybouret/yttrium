
#include "y/stream/proc/input.hpp"
#include "y/program.hpp"
#include "y/sequence/vector.hpp"
#include "y/string/tokenizer.hpp"
#include "y/system/exception.hpp"
#include "y/container/algo/crop.hpp"
#include "y/container/algo/search.hpp"
#include "y/associative/suffix/set.hpp"
#include "y/vfs/local/fs.hpp"
#include <cstring>
#include <iomanip>

using namespace Yttrium;

namespace
{
    static inline
    String GetPortName(const String &descr)
    {
        Vector<String>  words;
        Tokenizer::AppendTo(words,descr,' ');
        if(words.size()<=0) throw Exception("invalid description='%s'", descr.c_str());
        return words[1];
    }

    typedef SuffixSet<String,String> PortDeps;

    void GetPortDeps(PortDeps &     portDeps,
                     const String & portName)
    {
        static const char   buffer[]="Dependencies:";
        static const size_t buflen = strlen(buffer);

        // load content from depsCmd
        const String    depsCmd = "port deps " + portName;
        Vector<String>  output; ProcInput::AppendTo(output,depsCmd);

        // process each line
        for(size_t i=2;i<=output.size();++i)
        {
            const String &line = output[i];
            const char   *mark = strstr(line.c_str(), "Dependencies:");
            if(mark)
            {
                const String   data = mark+buflen;
                Vector<String> deps; Tokenizer::AppendTo(deps,data,',');
                for(size_t i=1;i<=deps.size();++i)
                {
                    String &dep = deps[i];
                    Algo::Crop(dep,isspace);
                    if(portDeps.search(dep)) continue;
                    if(!portDeps.insert(dep)) throw Exception("couldn't insert in portDeps database!!");
                    std::cerr << std::setw(32) << dep.c_str() << " <- " << portName << std::endl;
                }
            }
        }
    }

}

Y_Program()
{
    if(argc<=1)
    {
        std::cerr << "usage: " << program << " port [regexp]" << std::endl;
        return 1;
    }

    const String         primary = argv[1]; Algo::Crop( Coerce(primary), isspace);
    const Vector<String> installed(4096,AsCapacity);
    {
        String cmd = "port installed";
        if(argc>2) cmd += " '" + String(argv[2]) + "'";
        ProcInput::AppendTo( Coerce(installed), cmd);
    }

    const size_t   numPorts = installed.size();
    PortDeps       portDeps;
    for(size_t i=2;i<=numPorts;++i)
    {
        const String &  descr    = installed[i];
        const String    portName = GetPortName(descr);
        GetPortDeps(portDeps,portName);
    }
    

}
Y_End()
