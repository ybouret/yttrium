#include "y/stream/proc/input.hpp"
#include "y/program.hpp"
#include "y/sequence/vector.hpp"
#include "y/string/tokenizer.hpp"
#include "y/jive/pattern/matcher.hpp"
#include "y/system/exception.hpp"
#include "y/container/algo/crop.hpp"
#include "y/container/algo/search.hpp"
#include "y/associative/suffix/set.hpp"
#include "y/graphviz/vizible.hpp"
#include "y/jive/pattern/vfs.hpp"
#include "y/vfs/local/fs.hpp"
#include <cstring>

using namespace Yttrium;


namespace {

    typedef SuffixSet<String,String> PortDB;

    class Port : public Object, public GraphViz::Vizible
    {
    public:
        typedef CxxListOf<Port> List;

        static bool Installed(const String &portName)
        {
            const String   cmd = "port installed " + portName;
            Vector<String> lines;
            switch(ProcInput::AppendTo(lines,cmd).size())
            {
                case 0: throw Specific::Exception(portName.c_str(),"installed invalid answer");
                case 1: return false;
                default: break;
            }
            return true;
        }

        explicit Port(const String &portName,
                      size_t       &depth,
                      PortDB       &missing) :
        name(portName),
        deps(),
        next(0),
        prev(0)
        {
            assert(missing.search(portName));
            Core::Indent(std::cerr, depth<<1, ' ') << "|_[" << name << "]" << std::endl;
            ++depth;
            Vector<String> fields;
            {
                const String   sep = "Dependencies:";
                const String   cmd = "port deps " + portName;
                ProcInput      fp(cmd);
                String         line;
                while( fp.gets(line) )
                {
                    Vector<size_t> out;
                    Algo::Search(out,line,sep);
                    switch( out.size() )
                    {
                        case 0: continue;
                        case 1: break;
                        default:
                            throw Specific::Exception(name.c_str(),"invalid deps answer '%s'", line.c_str());
                    }
                    const String data = &line[out[1]+sep.size()];
                    Tokenizer::AppendTo(fields,data, ',');
                }
            }
            const size_t nf = fields.size();
            for(size_t i=1;i<=nf;++i)
            {
                const String &sub = Algo::Crop(fields[i], isspace);
                if( missing.search(sub) ) continue;
                if( Installed(sub) )      continue;
                missing.insert(sub);
                deps.pushTail( new Port(sub,depth,missing) );
            }
            --depth;
        }

        void viz(OutputStream &fp) const
        {
            for(const Port *sub=deps.head;sub;sub=sub->next)
            {
                sub->viz(fp);
            }
            Node(fp,this) << '[';
            Label(fp,name);
            fp << ",shape=box";
            fp << ']';
            Endl(fp);
            for(const Port *sub=deps.head;sub;sub=sub->next)
            {
                Arrow(fp, this, sub);
                Endl(fp);
            }
        }

        void graphViz(OutputStream &fp) const
        {
            Enter(fp, "G");
            viz(fp);
            Leave(fp);
        }


        const String name;
        List         deps;
        Port *       next;
        Port *       prev;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Port);
    };
}

Y_Program()
{
    if(argc<=1)
    {
        std::cerr << "usage: " << program << " portname" << std::endl;
        return 0;
    }

    LocalFS &fs = LocalFS::Instance();
    fs.tryRemoveFile("missing.dot");
    fs.tryRemoveFile("missing.png");


    const String   name  = argv[1];
    if( Port::Installed(name) )
    {
        std::cerr << "[" << name << "] is installed !" << std::endl;
        return 0;
    }

    PortDB         missing; missing.insert(name);
    size_t         depth = 1;
    AutoPtr<Port>  port  = new Port(name,depth,missing);
    std::cerr << "Rendering..." << std::endl;
    std::cerr.flush();
    GraphViz::Vizible::DotToPng("missing.dot", *port);

}
Y_End()

