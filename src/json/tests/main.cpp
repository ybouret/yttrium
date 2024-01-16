#include "y/json/parser.hpp"
#include "y/program.hpp"

using namespace Yttrium;

Y_Program()
{
    JSON::Parser &json = JSON::Parser::Instance();
    std::cerr << "LifeTime: " << json.LifeTime << std::endl;

    json.renderGraphViz();
}
Y_End()
