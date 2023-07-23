
#ifndef Y_Container_Operating_Included
#define Y_Container_Operating_Included 1

#include "y/container/writable.hpp"
#include "y/type/destruct.hpp"

namespace Yttrium
{
    namespace Core
    {

        class Operating
        {
        public:
            class Code;
            typedef void (*Build)(void *, void *);
            typedef void (*Smash)(void *);

        protected:
            explicit Operating(void *       blockAddr,
                               const size_t numBlocks,
                               const size_t blockSize,
                               Build        build,
                               void        *param,
                               Smash        smash);

        public:
            virtual ~Operating() noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Operating);
            Code *code;
        };
    }

    template <typename T> class Operating :
    public Core::Operating
    {
    public:
        Y_ARGS_EXPOSE(T);


        inline virtual ~Operating() noexcept { }
        inline explicit Operating(void        *workspace,
                                  const size_t numBlocks) :
        Core::Operating(workspace,numBlocks,sizeof(T),Build,0,Smash)
        {
        }

        template <typename U>
        inline explicit Operating(void        *workspace,
                                  const size_t numBlocks,
                                  U           &arguments) :
        Core::Operating(workspace,numBlocks,sizeof(T),Build1<U>,(void*)&arguments,Smash)
        {
        }
        

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Operating);

        static inline void Build(void *addr, void *)  { assert(0!=addr); new (addr) MutableType(); }
        static inline void Smash(void *addr) noexcept { assert(0!=addr); Destruct(static_cast<MutableType *>(addr)); }

        template <typename U>
        static inline void Build1(void *addr, void *args)
        {
            assert(0!=addr);
            assert(0!=args);
            new (addr) MutableType( *static_cast<U*>(args) );
        }



    };
}

#endif

