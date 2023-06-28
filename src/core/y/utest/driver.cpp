
#include "y/utest/driver.hpp"
#include "y/memory/out-of-reach.h"

namespace Yttrium
{
    UnitTest:: ~UnitTest() noexcept
    {

    }

    UnitTest:: UnitTest(const char *name_, proc func_) noexcept :
    name(name_),
    func(func_)
    {
        assert(NULL!=name);
        assert(NULL!=func);
    }

    void UnitTest:: display(std::ostream &os, const size_t width) const
    {
        const size_t w = strlen(name);assert(w<=width);
        const size_t d = width-w;
        const size_t prolog = d/2;
        const size_t epilog = d-prolog;
        os << '[';
        for(size_t i=0;i<prolog;++i) os << ' ';
        os << name;
        for(size_t i=0;i<epilog;++i) os << ' ';
        os << ']' << std::endl;
    }


    bool UnitTest:: is_near(const char *other) const noexcept
    {
        assert(NULL!=other);

        const char *big    = name;
        const char *little = other;

        if(strlen(little)>strlen(big))
        {
            Swap(big,little);
        }

        return NULL!=strstr(big,little);
    }


    void UnitTests:: clear() noexcept
    {
        Y_OutOfReach_Zero(utest,sizeof(UnitTest)*capacity);
    }

    UnitTests:: ~UnitTests() noexcept
    {
        clear();
    }

    UnitTests:: UnitTests(void *addr, const size_t size) noexcept :
    utest( static_cast<UnitTest*>(addr) ),
    count(0),
    width(0),
    capacity(size)
    {
        assert(NULL!=addr);
        clear();
    }

    UnitTest *UnitTests:: query(const char *name) noexcept
    {
        assert(NULL!=name);
        for(size_t i=0;i<count;++i)
        {
            UnitTest *t = &utest[i]; assert(NULL!=t->name);
            if( 0 == strcmp(t->name,name) )
            {
                return t;
            }
        }
        return NULL;
    }


    int  UnitTests:: operator()(const char     *name,
                                UnitTest::proc func) noexcept
    {
        assert(NULL!=name);
        assert(NULL!=func);

        if(count>=capacity) {
            std::cerr << "*** Exceeded capacity of #" << capacity << " unit tests!" << std::endl;
            return 1;
        }

        if(query(name)) {
            std::cerr << "*** Multiple test name [" << name << "]" << std::endl;
            return 1;
        }

        const size_t w = strlen(name);
        new( &utest[count++] ) UnitTest(name,func);
        if(w>width) width = w;
        return 0;
    }

    static inline int compare_unit_tests(const void *lhs, const void *rhs) noexcept
    {
        assert(NULL!=lhs);
        assert(NULL!=rhs);
        return strcmp(static_cast<const UnitTest *>(lhs)->name,
                      static_cast<const UnitTest *>(rhs)->name);

    }

    int UnitTests:: operator()(int      argc,
                               char **  argv) noexcept
    {

        qsort(utest,count,sizeof(UnitTest),compare_unit_tests);
        const char *program = argv[0];
        if(argc<=1)
        {
            std::cout << "#" << program << " = " << count << std::endl;
            for(size_t i=0;i<count;++i)
            {
                utest[i].display(std::cout << '\t',width);
            }
            std::cout << "#" << program << " = " << count << std::endl;
            return 0;
        }
        else
        {
            const char *name = argv[1]; assert(name);
            UnitTest   *test = query(name);
            if(test)
            {
                return test->func(--argc,++argv);
            }
            else
            {
                std::cerr << "No [" << name << "] in " << argv[0] << std::endl;
                bool first = true;
                for(size_t i=0;i<count;++i)
                {
                    const UnitTest &test = utest[i];
                    if(test.is_near(name))
                    {
                        if(first)
                        {
                            std::cerr << "near match:" << std::endl;
                            first = false;
                        }
                        test.display(std::cerr << '\t',width);
                    }
                }
                return 1;
            }
        }
    }




}
