
#include "y/mkl/fit/variable/interface.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace MKL
    {

        namespace Fit
        {

            Variable:: ~Variable() noexcept
            {
            }

            const String & Variable:: key() const noexcept
            {
                return *this;
            }


#if 0
            static inline
            size_t checkIndex(const String &name, const size_t indx)
            {
                if(indx<=0) throw Specific::Exception("Fit::Variable","'%s' with indx=0",name.c_str());
                return indx;
            }
#endif

            Variable:: Variable(const String &name) :
            String(name)
            {
            }

            Variable:: Variable(const char *name) :
            String(name)
            {
            }

            Variable:: Variable(const char name) :
            String(name)
            {
            }

            std::ostream & operator<<(std::ostream &os, const Variable &v)
            {
                //const String &id = v;
                //os << id << '@' << v.indx;
                os << '@' << v.idx();
                return os;
            }

        }

    }

}

