
#include "y/graphviz/vizible.hpp"
#include "y/text/hexadecimal.hpp"
#include "y/text/hexadecimal.hpp"
#include "y/text/ascii/embedding.hpp"
#include "y/text/ops.hpp"

namespace Yttrium
{
    namespace GraphViz
    {
        Vizible:: ~Vizible() noexcept
        {
        }

        Vizible:: Vizible() noexcept
        {
        }

        void Vizible:: Enter(OutputStream &fp, const String &graphName)
        {
            fp << "digraph " << graphName << " {\n";
        }

        void Vizible:: Enter(OutputStream &fp, const char *graphName)
        {
            const String _(graphName);
            Enter(fp,_);
        }

        void Vizible:: Leave(OutputStream &fp)
        {
            fp << "}\n";
        }


        OutputStream & Vizible:: Addr(OutputStream &fp, const void *ptr)
        {
            return fp << 'n' << Hexadecimal::Address(ptr);
        }

        OutputStream & Vizible:: Node(OutputStream &fp, const Vizible *viz)
        {
            return Addr(fp,viz);
        }


        OutputStream & Vizible:: Endl(OutputStream &fp)
        {
            return fp << ";\n";
        }

        static inline
        void emitASCII(OutputStream &fp, const char *msg, size_t len)
        {
            assert(Good(msg,len));
            while(len-- > 0)
            {
                const uint8_t byte = uint8_t(*(msg++));
                const char   *data = ASCII::Embedding::Char[byte];
                //std::cerr << "emit '" << data << "'" << std::endl;
                fp << data;
            }
        }

        OutputStream & Vizible:: Text(OutputStream &fp, const String &s)
        {
            emitASCII(fp,s(),s.size());
            return fp;
        }

        OutputStream & Vizible:: Text(OutputStream &fp, const char *msg)
        {
            emitASCII(fp,msg,StringLength(msg));
            return fp;
        }


        OutputStream & Vizible:: Arrow(OutputStream &fp, const Vizible *src, const Vizible *dst)
        {
            return Node(Node(fp,src) << " -> ",dst);
        }


        static inline
        void emitLabel(OutputStream &fp, const char *msg, size_t len)
        {
            assert(Good(msg,len));
            fp << "label=\"";
            emitASCII(fp,msg,len);
            fp << "\"";
        }

        OutputStream  & Vizible::Label(OutputStream &fp, const String &id)
        {
            emitLabel(fp,id(),id.size());
            return fp;
        }

        OutputStream  & Vizible::Label(OutputStream &fp, const char *id)
        {
            emitLabel(fp,id,StringLength(id));
            return fp;
        }

        OutputStream  & Vizible::Label(OutputStream &fp, const void *msg, const size_t len)
        {
            assert(Good(msg,len));
            emitLabel(fp,static_cast<const char*>(msg),len);
            return fp;
        }

    }
}


#include "y/graphviz/color-scheme.hpp"

namespace Yttrium
{
    namespace GraphViz
    {

        String Vizible:: Color(const String &scheme, size_t indx)
        {
            String res = '"';
            const size_t len = scheme.size();
            if(len>0)
            {
                const ColorScheme *s = ColorScheme::Get(scheme.c_str()); assert(0!=s); assert(scheme==s->name);
                const size_t       n = s->size; assert(n>0);
                while(indx>n)  indx -= n;
                while(indx<=0) indx += n;
                res << '/' << scheme << '/';
            }
            res += Formatted::Get("%lu", static_cast<unsigned long>(indx) );
            res += '"';
            return res;
        }

        String Vizible:: Color(const char *scheme, size_t indx)
        {
            const String _(scheme);
            return Color(_,indx);
        }

    }
}

#include "y/vfs/local/fs.hpp"
#include "y/system/exception.hpp"
#include "y/stream/libc/output.hpp"

namespace Yttrium
{
    namespace GraphViz
    {
        void Vizible:: Render(const String &pngFile,
                              const String &dotFile,
                              const bool    keepDot)
        {
            String cmd = "dot -T png -o " + pngFile + " " + dotFile;
            if( 0 != system( cmd() ) )
            {
                std::cerr << "*** failure to execute [" << cmd << "]" << std::endl;
            }
            else
            {
                if(!keepDot)
                {
                    static VFS &fs = LocalFS::Instance();
                    (void) fs.tryRemoveFile(dotFile);
                }
            }
        }

        OutputStream * Vizible:: OpenFile(const String &dotFile)
        {
            const String ext = VFS::Extension(dotFile);
            if(ext != ".dot") throw Specific::Exception("Vizible::OpenFile","invalid extension '%s'", ext() );
            return new OutputFile(dotFile);
        }

        void Vizible:: RenderPNG(const String &dotFile, const bool keepDot)
        {
            String pngFile = VFS::ChangedExtension("png", dotFile);
            Render(pngFile,dotFile,keepDot);
        }
    }
}
