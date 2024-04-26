
#include "y/ink/image/format/bmp.hpp"
#include "y/ink/image/format/jpeg.hpp"
#include "y/ink/image/codecs.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;



Y_UTEST(image)
{

    Ink::Codecs & IMG = Ink::Codecs::Instance();

    std::cerr << IMG.callSign() << std::endl;

    IMG( new Ink::FormatBMP()  );
    IMG( new Ink::FormatJPEG() );


}
Y_UDONE()

