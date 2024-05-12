#include "y/ink/pixmap.hpp"
#include "y/utest/run.hpp"
#include "y/concurrent/loop/crew.hpp"
#include "y/ink/image/codecs.hpp"
#include "y/color/ramp/black-and-white.hpp"
#include "y/vfs/local-fs.hpp"

using namespace Yttrium;
using namespace Ink;

namespace
{
    typedef Pixmap<float> PixmapType;

    class Patch : public PixmapType
    {
    public:
        static const uint32_t Width  = 5;
        static const uint32_t Pixels = Width*Width;
        static const uint32_t Count  = (1<<Pixels);
        //static const size_t   DataSize = Pixels * sizeof(Type);

        explicit Patch() : 
        PixmapType(Width,Width),
        data( &(*this)[0][0] ),
        ramp()
        {}
        virtual ~Patch() noexcept {}

        bool encode(const uint32_t word) noexcept
        {
            //memset(data,0,DataSize);
            uint32_t mask = 0x01;
            size_t   npix = 0;
            for(size_t i=0;i<Pixels;++i,mask<<=1)
            {
                if(0!=(mask&word))
                {
                    data[i] = 1;
                    ++npix;
                }
                else
                {
                    data[i] = 0;
                }
            }
            return npix>=4;
        }

        Type * const         data;
        Color::BlackAndWhite ramp;


    private:
        Y_DISABLE_COPY_AND_ASSIGN(Patch);
    };

}

//#include "y/jive/pattern/vfs.hpp"

Y_UTEST(make5)
{
    Concurrent::Topology   topo;
    Concurrent::SharedLoop crew = new Concurrent::Crew(topo);
    Slabs                  par( crew );
    Codec  &               IMG = Ink::Codecs::Std();
    Y_USHOW(Patch::Count);
    VFS &fs = LocalFS::Instance();
    const String outDir = "make5/";
    fs.makeDirectory(outDir,true);
    {
        AutoPtr<VFS::Scanner> scan = fs.openDirectory(outDir);
        VFS::Entries          toRemove;
        for(;;)
        {
            AutoPtr<VFS::Entry> ep = scan->get();
            if(!ep.isValid()) break;
            //std::cerr << ep->path << " " << ep->typeText() << std::endl;
            if(!ep->isReg())  continue;
            const char * ext = VFS::Extension(ep->path);
            if(!ext) continue;
            const String Ext = ++ext;
            if("pbm" != Ext) continue;
            std::cerr << "found '" << ep->path << "'" << std::endl;
            toRemove.pushTail( ep.yield() );
        }
        std::cerr << "Cleaning #" << toRemove.size << std::endl;
        for(const VFS::Entry *ep=toRemove.head;ep;ep=ep->next)
        {
            fs.tryRemoveFile(ep->path);
        }
    }

    return 0;

    Patch    patch;
    uint32_t num = 0;
    Options  opt; opt << "zoom=8";
    for(uint32_t i=0;i<Patch::Count;++i)
    {
        if(patch.encode(i))
        {
            ++num;
            const String fileName = outDir + Formatted::Get("patch%08x.pbm",i);
            IMG.save(patch, fileName, &opt, par, patch.ramp);
            if(num>=100) return 0;
        }
    }

}
Y_UDONE()

