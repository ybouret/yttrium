
//! \file

#ifndef Y_Net_Parameter_Included
#define Y_Net_Parameter_Included 1


#include "y/string.hpp"
#include "y/ptr/ark.hpp"
#include "y/associative/suffix/set.hpp"


namespace Yttrium
{

    namespace Network
    {
        typedef ArkPtr<String,const String> Label;

        template <typename T>
        class Parameter
        {
        public:
            Y_ARGS_EXPOSE(T,Type);

            template <typename NAME> inline
            explicit Parameter(ConstType v, const NAME & l) :
            value(v),
            label( new String(l) )
            {
            }

            //! duplicate with shared copy
            inline Parameter(const Parameter &_) noexcept : value(_.value), label(_.label) {}

            //! cleanup
            inline virtual ~Parameter() noexcept {}

            inline const String & key() const noexcept { return *label; }

            ConstType   value;
            const Label label;

        private:
            Y_DISABLE_ASSIGN(Parameter);
        };

        


    }

}

#endif

