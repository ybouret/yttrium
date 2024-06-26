#include "y/json/parser.hpp"
#include "y/program.hpp"

using namespace Yttrium;

Y_Program()
{
    JSON::Parser &json = JSON::Parser::Instance();
    std::cerr << "LifeTime: " << json.LifeTime << std::endl;

    json.renderGraphViz();
    JSON::Value jv;
    if(argc>1)
    {
        json.load(jv,Jive::Module::OpenFile(argv[1]) );
    }
}
Y_End()
