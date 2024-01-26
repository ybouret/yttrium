
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
                Coerce(indx) = 0;
            }

            const String & Variable:: key() const noexcept
            {
                return *this;
            }


            static inline
            size_t checkIndex(const String &name, const size_t indx)
            {
                if(indx<=0) throw Specific::Exception("Fit::Variable","'%s' with indx=0",name.c_str());
                return indx;
            }

            Variable:: Variable(const String &name, const size_t i) :
            String(name), indx( checkIndex(*this,i) )
            {
            }

            Variable:: Variable(const char *name, const size_t i) :
            String(name), indx( checkIndex(*this,i) )
            {
            }

            Variable:: Variable(const char name, const size_t i) :
            String(name), indx( checkIndex(*this,i) )
            {
            }

            std::ostream & operator<<(std::ostream &os, const Variable &v)
            {
                //const String &id = v;
                //os << id << '@' << v.indx;
                os << '@' << v.indx;
                return os;
            }

        }

    }

}

