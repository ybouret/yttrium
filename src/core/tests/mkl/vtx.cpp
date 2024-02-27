
#include "y/mkl/v4d.hpp"
#include "y/utest/run.hpp"
#include "y/utest/timing.hpp"
#include "y/random/in2d.hpp"
#include "y/random/in3d.hpp"
#include "y/stream/libc/output.hpp"

using namespace Yttrium;

Y_UTEST(mkl_vtx)
{
    Random::Rand ran;

    {
        OutputFile fp("circle.dat");
        for(size_t i=100;i>0;--i)
        {
            V2D<float> v = Random::OnCircle< V2D<float> >(ran);
            fp("%g %g\n",v.x,v.y);
        }
    }

    {
        OutputFile fp("disk.dat");
        for(size_t i=100;i>0;--i)
        {
            V2D<double> v = Random::InDisk< V2D<double> >(ran);
            fp("%g %g\n",v.x,v.y);
        }
    }

    {
        OutputFile fp("sphere.dat");
        for(size_t i=100;i>0;--i)
        {
            V3D<double> v = Random::OnSphere< V3D<double> >(ran);
            fp("%g %g %g\n",v.x,v.y,v.z);
        }
    }


}
Y_UDONE()
