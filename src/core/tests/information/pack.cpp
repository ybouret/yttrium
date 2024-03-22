
#include "y/utest/run.hpp"
#include "y/information/entropic/alphabet.hpp"
#include "y/information/entropic/model/huffman.hpp"
#include "y/sequence/vector.hpp"
#include "y/random/shuffle.hpp"

using namespace Yttrium;
using namespace Information;

#include "y/string.hpp"

static const char     Ch[] = { 'a', 'e', 'i', 'o', 'u' };
static const unsigned Nc[] = { 12,   42,   9,  30,  7  };
static const unsigned NN   = sizeof(Ch)/sizeof(Ch[0]);

template <typename SEQ> static inline
void loadAlpha(Entropic::Alphabet &alpha, const SEQ &msg, Entropic::Model &model)
{
    std::cerr << std::endl;
    alpha.reset();
    alpha.display(std::cerr);
    for(size_t i=1;i<=msg.size();++i)
    {
        alpha.update( alpha[msg[i]] );
    }
    alpha.display(std::cerr);
    alpha.commit(model);
    alpha.display(std::cerr);
    const size_t nb = alpha.outputBytesFor(msg);
    std::cerr << "OutputBytes: " << nb << "/" << msg.size() << std::endl;
    
#if 1
    for(unsigned i=0;i<256;++i)
    {
        alpha.update( alpha[i] );
    }
    //alpha.display(std::cerr);
#endif
}

Y_UTEST(info_pack)
{
    Random::Rand ran;
    String       msg;
    for(unsigned i=0;i<NN;++i)
    {
        const char c=Ch[i];
        for(unsigned j=Nc[i];j>0;--j)
            msg << c;
    }

    Random::Shuffle::Range(msg,ran);

    Entropic::Huffman huff;

    Entropic::Alphabet alphaBlock(Entropic::Alphabet::Precompiling);
    Entropic::Alphabet alphaFlux1(Entropic::Alphabet::SingleStream);
    Entropic::Alphabet alphaFlux2(Entropic::Alphabet::MultiStreams);

    loadAlpha(alphaBlock,msg,huff);
    loadAlpha(alphaFlux1,msg,huff);
    loadAlpha(alphaFlux2,msg,huff);






}
Y_UDONE()
