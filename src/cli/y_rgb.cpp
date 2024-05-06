

#include "y/program.hpp"
#include "y/stream/libc/input.hpp"
#include "y/string.hpp"
#include "y/string/tokenizer.hpp"
#include "y/associative/hash/map.hpp"
#include "y/sequence/vector.hpp"
#include "y/text/ascii/convert.hpp"
#include "y/container/algo/crop.hpp"
#include "y/color/rgb.hpp"

using namespace Yttrium;

static inline uint8_t GetField(const String      &field,
                               const char * const name)
{
    assert(0!=name);
    return ASCII::Convert::To<uint8_t>(field,name);
}

typedef Color::RGB<uint8_t> RGB;

Y_Program()
{
    const char * field[4] = { 0, "red", "blue", "green" };

    HashMap<String,RGB> cmap;
    if(argc>1)
    {
        InputFile     fp( argv[1] );
        String         line;
        Vector<String> words(16,AsCapacity);
        while( fp.gets(line) )
        {
            words.free();
            Tokenizer::AppendTo(words, line, " \t", 2);if(words.size()<4) continue;
            for(size_t i=words.size();i>0;--i) Algo::Crop(words[i], isspace);

            String name;
            for(size_t i=4;i<=words.size();++i)
            {
                String &word = words[i];
                word[1] = toupper(word[1]);
                name << word;
            }
            if(cmap.search(name)) continue;

            //std::cerr << "name='" << name << "'" << std::endl;
            RGB color;
            uint8_t * const target = static_cast<uint8_t*>(Memory::OutOfReach::Addr(&color.r))-1;
            for(size_t i=1;i<=3;++i)
            {
                const String which = field[i] + ( " of " + name);
                target[i] = GetField(words[i], which.c_str());
            }
            std::cerr << "'" << name << "'=" << color << std::endl;
        }
    }

}
Y_End()
