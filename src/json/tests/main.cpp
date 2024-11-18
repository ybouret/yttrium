#include "y/json/parser.hpp"
#include "y/json/compiler.hpp"
#include "y/program.hpp"

using namespace Yttrium;

Y_Program()
{
#if 0
    JSON::Parser &json = JSON::Parser::Instance();
    std::cerr << "LifeTime: " << json.LifeTime << std::endl;

    json.renderGraphViz();
    JSON::Value jv;
    if(argc>1)
    {
        json.load(jv,Jive::Module::OpenFile(argv[1]) );
    }
#endif

    JSON::Compiler jcmp;
    JSON::Value    jv;

    if(argc>1)
    {
        jcmp.load(jv, Lingo::Module::OpenFile(argv[1]));
    }
}
Y_End()
