#include "y/json/compiler.hpp"
#include "y/program.hpp"

using namespace Yttrium;

Y_Program()
{

    JSON::Compiler json; json.verbose = true;
    JSON::Value    jv;

    if(argc>1)
    {
        json.load(jv, Lingo::Module::OpenFile(argv[1]));
    }
}
Y_End()
