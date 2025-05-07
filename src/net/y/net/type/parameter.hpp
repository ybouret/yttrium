
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
            explicit Parameter(ConstType &v, const NAME & l) :
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

#define Y_Net_Decl(CLASS,TYPE) \
/**/    class CLASS : public Parameter<TYPE> \
/**/    {\
/**/      public:\
/**/        typedef Parameter<TYPE>               BaseClass; \
/**/        typedef SuffixSet<String,const CLASS> Set;       \
/**/        virtual ~CLASS() noexcept;                       \
/**/        CLASS(const CLASS &_) noexcept;                  \
/**/        template <typename NAME> inline                  \
/**/        explicit CLASS(ConstType v, const NAME &l) :     \
/**/            BaseClass(v,l) {}                            \
/**/      private:\
/**/        Y_DISABLE_ASSIGN(CLASS);\
/**/    };\
/**/    class CLASS##Set : public CLASS::Set \
/**/    {\
/**/      public:\
/**/        static const char * const CallSign;\
/**/        explicit CLASS##Set(const Label &unk);\
/**/        virtual ~CLASS##Set() noexcept;\
/**/        template <typename NAME> inline \
/**/        void operator()(CLASS::ConstType value, const NAME &label)\
/**/        {\
/**/            const CLASS param(value,label); add(param);\
/**/        }\
/**/        const String & operator[](CLASS::ConstType) const noexcept;\
/**/      private:\
/**/        Y_DISABLE_COPY_AND_ASSIGN(CLASS##Set);\
/**/        const Label unknown; \
/**/        void add(const CLASS &param); \
/**/    }

#define Y_Net_Impl(CLASS,TYPE)                                   \
/**/    CLASS:: ~CLASS() noexcept {}                             \
/**/    CLASS:: CLASS(const CLASS &_) noexcept : BaseClass(_) {} \
/**/    CLASS##Set:: ~CLASS##Set() noexcept {}                   \
/**/    CLASS##Set::  CLASS##Set(const Label &_) : CLASS::Set(), unknown(_) {}             \
/**/    void CLASS##Set:: add(const CLASS &param) \
/**/    {\
/**/       if(!insert(param)) throw Specific::Exception(CallSign,"multiple '%s'", param.label->c_str());\
/**/    }\
/**/    const String & CLASS##Set:: operator[](CLASS::ConstType) const noexcept\
/**/    {\
/**/        return *unknown;\
/**/    }\
/**/    const char * const CLASS##Set:: CallSign = "Network::" #CLASS

#define Y_Net_Add(SET,LABEL) SET(LABEL,#LABEL)

    }

}

#endif

