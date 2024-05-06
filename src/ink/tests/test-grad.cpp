

#include "y/color/ramp/flexible.hpp"
#include "y/color/ramp/gradation.hpp"
#include "y/color/ramp/map8.hpp"

#include "y/ink/ops/filter/prewitt.hpp"
#include "y/ink/ops/filter/sobel.hpp"
#include "y/ink/ops/filter/scharr.hpp"
#include "y/ink/ops/gradient/filters.hpp"

#include "y/utest/run.hpp"
#include "y/ink/image/codecs.hpp"
#include "y/concurrent/loop/crew.hpp"
#include "y/color/grayscale.hpp"
#include "y/ink/ops/gradient/thinner.hpp"

#include "y/data/small/heavy/list/coop.hpp"
#include "y/data/small/heavy/list/bare.hpp"

#include "y/color/rgb/x11.hpp"
#include "y/color/ramp/map-index.hpp"



namespace Yttrium
{
    namespace Ink
    {

        typedef Small::CoopHeavyList<Coord> CoordList;
        typedef CoordList::NodeType         CoordNode;
        typedef CoordList::ProxyType        CoordBank;
        typedef Small::BareHeavyList<Coord> CoordStore;

        class Edge : public Object, public CoordList
        {
        public:
            enum Connectivity
            {
                Connect4 = 4,
                Connect8 = 8
            };
            static const Coord Delta[8];

            typedef CxxListOf<Edge> List;
            explicit Edge(const size_t i, const CoordBank &bank) :
            CoordList(bank),
            label(i),
            next(0),
            prev(0)
            {
            }

            virtual ~Edge() noexcept
            {
            }

            const size_t label;
            Edge        *next;
            Edge        *prev;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Edge);
        };

        class Edges : public Proxy<const Edge::List>
        {
        public:

            explicit Edges() : Proxy<const Edge::List>(), edges(), cbank() {}
            virtual ~Edges() noexcept {}

            static inline void ZeroLabel(Slab &slab, Pixmap<size_t> &label)
            {
                static const size_t zero = 0;
                slab.load(label,zero);
            }

            void build(Slabs                    &slabs,
                       Pixmap<size_t>           &label,
                       const Pixmap<uint8_t>    &force,
                       const Edge::Connectivity &conn)
            {
                //______________________________________________________________
                //
                //
                // initializing
                //
                //______________________________________________________________
                edges.release();
                slabs(ZeroLabel,label);
                //cbank->ensure(force.n);

                const unit_t h = force.h;
                const unit_t w = force.w;
                for(unit_t j=0;j<h;++j)
                {
                    PixRow<size_t>        &label_j = label[j];
                    const PixRow<uint8_t> &force_j = force[j];
                    for(unit_t i=0;i<w;++i)
                    {
                        size_t &label_ji = label_j[i];
                        if(label_ji>0)  continue; // in another edge
                        if(force_j[i]<=0) continue; // nothing here
                        Edge         *edge = edges.pushTail( new Edge( edges.size+1, cbank) );
                        CoordList     scan(cbank);
                        const size_t  indx = edge->label;
                        scan     << Coord(i,j);
                        label_ji = indx;

                        while(scan.size>0)
                        {
                            const size_t n = scan.size;
                            for(size_t k=n;k>0;--k)
                            {
                                CoordNode  *node = edge->pushTail(scan.popTail());
                                const Coord here = **node; assert(indx==label[here]);

                                for(unsigned p=0;p<conn;++p)
                                {
                                    const Coord pos = here + Edge::Delta[p];
                                    if(!label.contains(pos)) continue;
                                    if(label[pos]>0) { assert(indx==label[pos]); continue; }
                                    if(force[pos]<=0) continue;
                                    scan       << pos;
                                    label[pos] = indx;
                                }
                            }
                        }
                        std::cerr << "#edge=" << edge->size << std::endl;
                    }
                }
                std::cerr << "#edges=" << edges.size << std::endl;



            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Edges);
            virtual ConstInterface & surrogate() const noexcept { return edges; }
            Edge::List edges;
            CoordBank  cbank;
        };

        const Coord Edge:: Delta[8] =
        {
            Coord( 1, 0),
            Coord( 0, 1),
            Coord(-1, 0),
            Coord( 0,-1),

            Coord( 1, 1), // +x, +y
            Coord(-1, 1), // -x, +y
            Coord(-1,-1), // -x, -y
            Coord( 1,-1)  // +x, -y
        };




    }
}


using namespace Yttrium;
using namespace Ink;

#include "y/stream/libc/output.hpp"


static inline
void processGrad(Slabs                  &par,
                 GradientMap<float>     &gmap,
                 const Gradient<float>  &grad,
                 GradientThinner        &thin,
                 const Pixmap<float>    &pxf,
                 const Color::Ramp      &cr)
{
    std::cerr << "Apply " << grad.name << std::endl;
    Codec &IMG = Codecs::Location();
    Histogram hist;
    gmap(par,grad,pxf);
    std::cerr << "gmap: " << gmap.nmin << " -> " << gmap.nmax << std::endl;
    thin(par,hist,gmap);
    (std::cerr << "saving..." << std::endl).flush();



    {
        {
            const Color::FlexibleRamp ramp(cr,gmap.nmin,gmap.nmax);
            const String fileName = "grad-" + grad.name + ".png";
            IMG.save(gmap.intensity,fileName, 0, par, ramp);
        }

        {
            Color::RampColor       bwc[2] = { Y_Black, Y_White };
            const Color::Gradation bwg(bwc,2);
            const Color::Map8      ramp(bwg);
            const String fileName = "thin-" + grad.name + ".png";
            IMG.save(thin.intensity,fileName, 0,par,ramp);
        }

        std::cerr << "building edges..." << std::endl;
        Edges edges;
        edges.build(par,Coerce(thin.label), thin.intensity, Edge::Connect8);
        {
            Color::RampColor       ecr[] = { Y_White, Y_Blue, Y_Red, Y_Green };
            const Color::MapIndex  icr(ecr,sizeof(ecr)/sizeof(ecr[0]));
            const String fileName = "edge-" + grad.name + ".png";
            IMG.save(thin.label,fileName, 0,par,icr);
        }

#if 0
        {
            const String fileName = "hist-" + grad.name + ".dat";
            hist.save(fileName);
        }

        {
            const String fileName = "sep-" + grad.name + ".dat";
            OutputFile fp(fileName);
            for(unsigned i=0;i<256;++i)
            {
                fp("%g %g\n", double(i), double( hist.separation(i)));
            }
            const uint8_t t = hist.threshold();
            std::cerr << "threshold= " << int(t) << std::endl;
        }
#endif

    }
}

Y_UTEST(grad)
{

    Concurrent::Topology   topo;
    Concurrent::SharedLoop crew = new Concurrent::Crew(topo);
    Slabs                  par( crew );
    Codecs &               IMG = Ink::Codecs::Std();

    static const RGBA Grad[] = {
        RGBA(0x00,0x00,0x00),
        RGBA(0x00,0x00,0xff),
        RGBA(0x00,0xff,0x00),
        RGBA(0xff,0x00,0x00),
        RGBA(0xff,0xff,0xff) };
    static const Color::Gradation cr(Grad,sizeof(Grad)/sizeof(Grad[0]));

    GradientFilters<float,Crux::Prewitt3> Prewitt3;
    GradientFilters<float,Crux::Prewitt5> Prewitt5;
    GradientFilters<float,Crux::Prewitt7> Prewitt7;

    GradientFilters<float,Crux::Sobel3>   Sobel3;
    GradientFilters<float,Crux::Sobel5>   Sobel5;
    GradientFilters<float,Crux::Sobel7>   Sobel7;

    GradientFilters<float,Crux::Scharr3>  Scharr3;
    GradientFilters<float,Crux::Scharr5>  Scharr5;


    if(argc>1)
    {
        Pixmap<RGBA>    img = IMG.load(argv[1],0);
        Pixmap<float>   pxf(par,Color::GrayScale::Pack<float,RGBA>,img);
        IMG.save(img, "img.png", 0);
        GradientMap<float>     gmap(img.w,img.h);
        GradientThinner        thin(img.w,img.h);

        processGrad(par,gmap,Prewitt3,thin,pxf,cr);
        return 0;
        processGrad(par,gmap,Prewitt5,thin,pxf,cr);
        processGrad(par,gmap,Prewitt7,thin,pxf,cr);
        processGrad(par,gmap,Sobel3,thin,pxf,cr);
        processGrad(par,gmap,Sobel5,thin,pxf,cr);
        processGrad(par,gmap,Sobel7,thin,pxf,cr);
        processGrad(par,gmap,Scharr3,thin,pxf,cr);
        processGrad(par,gmap,Scharr5,thin,pxf,cr);



    }




}
Y_UDONE()
