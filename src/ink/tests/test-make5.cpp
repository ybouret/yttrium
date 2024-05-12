#include "y/ink/pixmap.hpp"
#include "y/utest/run.hpp"
#include "y/concurrent/loop/crew.hpp"
#include "y/ink/image/codecs.hpp"
#include "y/color/ramp/black-and-white.hpp"

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

        explicit Patch() : PixmapType(Width,Width), data( &(*this)[0][0] ) {}
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

Y_UTEST(make5)
{
    Concurrent::Topology   topo;
    Concurrent::SharedLoop crew = new Concurrent::Crew(topo);
    Slabs                  par( crew );
    Codec  &               IMG = Ink::Codecs::Std();
    Y_USHOW(Patch::Count);

    Patch    patch;
    uint32_t num = 0;
    Options  opt; opt << "zoom=8";
    for(uint32_t i=0;i<Patch::Count;++i)
    {
        if(patch.encode(i))
        {
            ++num;
            const String fileName = Formatted::Get("patch%08x.pbm",i);
            IMG.save(patch, fileName, &opt, par, patch.ramp);
            if(num>=100) return 0;
        }
    }

}
Y_UDONE()

