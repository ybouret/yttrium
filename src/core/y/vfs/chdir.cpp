#include "y/vfs/vfs.hpp"
#include "y/data/small/heavy/list/solo.hpp"
#include "y/type/nullify.hpp"
#include "y/quantized.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace
    {
        typedef Small::SoloHeavyList<const String> DirList;

    }

    const char * const VFS:: ChangeDirectory:: CallSign = "VFS::ChangeDirectory";

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

        //! check
        inline void check()
        {
            if(size<=0) throw Specific:: Exception(CallSign, "empty state!!!");
            const String & last = **(tail);
            const String   here = vfs.getCWD();
            if(last!=here) throw Specific::Exception(CallSign, "in '%s'!='%s'", here.c_str(), last.c_str());
        }


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


    VFS::ChangeDirectory & VFS::ChangeDirectory:: operator<<(const String &dirName)
    {
        assert(0!=code);
        (*code) << dirName;
        try        { code->vfs.setCWD(dirName); }
        catch(...) { code->cutTail(); throw;    }
        return *this;
    }

    VFS::ChangeDirectory & VFS::ChangeDirectory:: operator<<(const char * const  dirName)
    {
        const String _(dirName);
        return (*this) << _;
    }

    const char *   VFS:: ChangeDirectory :: callSign() const noexcept { return CallSign; }
    size_t         VFS:: ChangeDirectory :: size()     const noexcept { assert(0!=code); return code->size; }
    const String & VFS:: ChangeDirectory:: operator[](const size_t iDir) const noexcept
    {
        assert(iDir>0);
        assert(iDir<=code->size);
        return **(code->fetch(iDir));
    }


    VFS::ChangeDirectory & VFS::ChangeDirectory:: up()
    {
        assert(0!=code);
        code->check();
        if(code->size>1)
        {
            
        }
        return *this;
    }

}
