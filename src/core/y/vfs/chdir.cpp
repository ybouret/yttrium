#include "y/vfs/vfs.hpp"
#include "y/data/small/heavy/list/solo.hpp"
#include "y/type/nullify.hpp"
#include "y/quantized.hpp"

namespace Yttrium
{
    namespace
    {
        typedef Small::SoloHeavyList<const String> DirList;

    }

    class VFS::ChangeDirectory::Code : public Quantized, public DirList
    {
    public:
        //! setup with cwd from vfs
        inline explicit Code(VFS &_) :  Quantized(), DirList(), vfs(_)
        {
            (*this) << vfs.getCWD();
        }

        //! copy other state
        inline explicit Code(const Code &other) : Quantized(), DirList(other), vfs(other.vfs) {}

        //! cleanup
        inline virtual ~Code() noexcept {}



        VFS &vfs; //! user's persistent vfs


    private:
        Y_DISABLE_ASSIGN(Code);
    };

    VFS:: ChangeDirectory:: ChangeDirectory(VFS &vfs) : code( new Code(vfs) )
    {
        std::cerr << "sizeof(Code)=" << sizeof(Code) << std::endl;
    }


    VFS:: ChangeDirectory:: ChangeDirectory(const ChangeDirectory &other) :
    code( new Code( *other.code) )
    {
    }


    VFS:: ChangeDirectory:: ~ChangeDirectory() noexcept
    {
        assert(0!=code);
        Nullify(code);
    }

    std::ostream & operator<<(std::ostream &os, const VFS::ChangeDirectory &self)
    {
        assert(0!=self.code);
        os << *(self.code);
        return os;
    }


}
