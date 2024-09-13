#include "y/stream/disk/file.hpp"
#include "y/type/nullify.hpp"
#include "y/object.hpp"

namespace Yttrium
{
    DiskFile:: ~DiskFile() noexcept
    {
        assert( 0 != code );
        Nullify(code);
    }

    class DiskFile:: Code : public Object
    {
    public:
        explicit Code() 
        {

        }

        virtual ~Code() noexcept
        {

        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Code);
    };


    DiskFile:: DiskFile(const char * const) : code( new Code() )
    {

    }

}

