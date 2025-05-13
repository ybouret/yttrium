#include "y/associative/named/variables.hpp"
#include "y/utest/run.hpp"



using namespace Yttrium;


Y_UTEST(associative_named_vars)
{

    typedef Named::Variable<int> iVar;

    Named::ByName<iVar> blv( new iVar("one",1) );
    Named::ByUUID<iVar> bvv( blv );

    std::cerr << blv << " / " << bvv << std::endl;
    Y_CHECK( & *blv == & *bvv );

    typedef Named::Variables<iVar> MyVars;
    MyVars vdb("bad",-1);

#if 1
    Y_SIZEOF(Named::Variable<int>);
    Y_SIZEOF(MyVars::Code);
    Y_SIZEOF(MyVars);
#endif

    Y_CHECK(vdb("one",1));
    Y_CHECK(vdb("two",2));
    Y_CHECK(vdb("hundred",100));

    std::cerr << vdb->byName << std::endl;
    std::cerr << vdb.uuid("one") << std::endl;
    std::cerr << vdb.uuid("two") << std::endl;
    std::cerr << vdb.uuid("three") << std::endl;
    std::cerr << vdb.uuid("hundred") << std::endl;

    std::cerr << vdb[1] << std::endl;
    std::cerr << vdb[100] << std::endl;
    std::cerr << vdb[7] << std::endl;

}
Y_UDONE()
