#include "y/singleton.hpp"
#include "y/utest/run.hpp"

using namespace Yttrium;

namespace
{
    class MyData : public Singleton<MyData>
    {
    public:
        static const char * const      CallSign;
        static const AtExit::Longevity LifeTime=77;
        

    private:
        Y_DISABLE_COPY_AND_ASSIGN(MyData);
        MyData() : Singleton<MyData>() { std::cerr << "+MyData" << std::endl; }
        virtual ~MyData() noexcept { std::cerr << "-MyData" << std::endl;}
        friend class Singleton<MyData>;
    };

    const char * const MyData:: CallSign = "MyData";
}

Y_UTEST(singleton)
{
    
    MyData &data = MyData::Instance();
    std::cerr << "data.callSign = " << data.callSign() << std::endl;
    
}
Y_UDONE()

