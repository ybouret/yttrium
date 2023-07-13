#include "y/singleton.hpp"
#include "y/utest/run.hpp"
#include "y/locking/nucleus.hpp"

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

    class SysData : public Singleton<SysData,Locking::Nucleus>
    {
    public:
        static const char * const      CallSign;
        static const AtExit::Longevity LifeTime=777;


    private:
        Y_DISABLE_COPY_AND_ASSIGN(SysData);
        SysData() : Singleton<SysData,Locking::Nucleus>() { std::cerr << "+SysData" << std::endl; }
        virtual ~SysData() noexcept { std::cerr << "-SysData" << std::endl;}
        friend class Singleton<SysData,Locking::Nucleus>;
    };

    const char * const SysData:: CallSign = "SysData";

}

Y_UTEST(singleton)
{
    
    MyData &data = MyData::Instance();
    std::cerr << "data.callSign = " << data.callSign() << std::endl;
    {
        Y_LOCK(MyData::Access);
    }


    SysData &sysData = SysData::Instance();
    std::cerr << "sysd.callSign = " << sysData.callSign() << std::endl;
    {
        Y_LOCK(SysData::Access);
    }

}
Y_UDONE()

