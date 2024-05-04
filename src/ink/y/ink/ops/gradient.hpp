//! \file

#ifndef Y_Ink_Gradient_Included
#define Y_Ink_Gradient_Included 1

#include "y/ink/ops/filter.hpp"

namespace Yttrium
{
    
    namespace Ink
    {

        namespace Crux
        {
            class Gradient : public Object, public Counted
            {
            public:

                const String &key() const noexcept;
                virtual ~Gradient() noexcept;
            protected:
                template <typename NAME> inline
                explicit Gradient(const NAME &id) : name(id) {}

            public:
                const String name;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Gradient);
            };
        }

        template <typename T>
        class Gradient : public Crux::Gradient
        {
        public:
            typedef Filter<T>                     FilterType;
            typedef ArkPtr<String,const Gradient> Handle;
            
            virtual const FilterType & dx() const noexcept = 0;
            virtual const FilterType & dy() const noexcept = 0;

            inline virtual ~Gradient() noexcept {}
        protected:
            template <typename NAME>
            inline explicit Gradient(const NAME &id) :
            Crux::Gradient(id) {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Gradient);
        };
    }

}

#endif

