#include "y/ink/bitmap.hpp"
#include "y/memory/out-of-reach.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

namespace Yttrium
{

    namespace Ink
    {

        

        template <typename T>
        struct PixRow
        {
            T * const    entry;
            const size_t width;
            inline size_t size() const noexcept { return width; }
            inline T & operator[](const unit_t j) const noexcept {
                return entry[j];
            }
        };

    }

}

Y_UTEST(bitmap)
{
    
    int               pixel[] = { 1, 2, 3, 4 };
    const size_t      count   = sizeof(pixel)/sizeof(pixel[0]);
    Ink::BitRow       row_    = { pixel, count };
    Ink::PixRow<int> &row     = Memory::OutOfReach::Conv< Ink::PixRow<int>, Ink::BitRow>(row_);

    std::cerr << "addr@ " << row.entry  << std::endl;
    std::cerr << "size= " << row.size() << std::endl;

    Ink::Bitmap bmp1(10,10,1);
    

}
Y_UDONE()
