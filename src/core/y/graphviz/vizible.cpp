
#include "y/graphviz/vizible.hpp"
#include "y/text/hexadecimal.hpp"
#include "y/text/hexadecimal.hpp"
#include "y/text/ascii/embedding.hpp"
#include "y/text/ops.hpp"

namespace Yttrium
{
    Vizible:: ~Vizible() noexcept
    {
    }

    Vizible:: Vizible() noexcept
    {
    }

    void Vizible:: Leave(OutputStream &fp)
    {
        fp << "}\n";
    }

    OutputStream & Vizible:: Node(OutputStream &fp, const Vizible *viz)
    {
        return fp << 'n' << Hexadecimal::Address(viz);
    }


    OutputStream & Vizible:: Endl(OutputStream &fp)
    {
        return fp << ";\n";
    }

    static inline
    void emitASCII(OutputStream &fp, const char *msg, size_t len)
    {
        assert(Good(msg,len));
        while(len-- > 0) fp << ASCII::Embedding::Char[ uint8_t(*(msg++))];
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


    OutputStream & Vizible:: node(OutputStream &fp) const
    {
        return Node(fp,this);
    }

    OutputStream & Vizible:: arrow(OutputStream &fp, const Vizible *dst) const
    {
        return Arrow(fp,this,dst);
    }

    void Vizible:: Render(const String &pngFile, const String &dotFile)
    {
        String cmd = "dot -T png -o " + pngFile + " " + dotFile;
        if( 0 != system( cmd() ) )
        {
            std::cerr << "*** failure to execute [" << cmd << "]" << std::endl;
        }
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

}
