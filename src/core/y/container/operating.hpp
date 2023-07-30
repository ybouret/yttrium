
#ifndef Y_Container_Operating_Included
#define Y_Container_Operating_Included 1

#include "y/container/writable.hpp"
#include "y/type/destruct.hpp"

namespace Yttrium
{
    
    namespace Core
    {
        //______________________________________________________________________
        //
        //
        //
        //! Anonymous operating objects
        //
        //
        //______________________________________________________________________
        class Operating
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            class Code;
            typedef void (*Build)(void *, void *); //!< constructor prototype
            typedef void (*Smash)(void *);         //!< destructor  prototype

        protected:
            //! setup
            /**
             \param blockAddr flat memory
             \param numBlocks objects to be setup in flat memory
             \param blockSize sizeof(object)
             \param build     constructor
             \param param     constructor parameters
             \param smash     destructor
             */
            explicit Operating(void *       blockAddr,
                               const size_t numBlocks,
                               const size_t blockSize,
                               Build        build,
                               void        *param,
                               Smash        smash);

        public:
            virtual ~Operating() noexcept; //!< cleanup, release all

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Operating);
            Code *code;
        };
    }

    //__________________________________________________________________________
    //
    //
    //
    //! Operating generic objects
    //
    //
    //__________________________________________________________________________
    template <typename T> class Operating :
    public Core::Operating
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        Y_ARGS_EXPOSE(T,Type); //!< aliases


        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________

        //! release and cleanup
        inline virtual ~Operating() noexcept { }

        //! setup numBlocks objects with default constructor
        inline explicit Operating(void        *workspace,
                                  const size_t numBlocks) :
        Core::Operating(workspace,numBlocks,sizeof(T),Build,0,Smash)
        {
            std::cerr << "building " << numBlocks << " @ItemSize=" << sizeof(T) << std::endl;
        }

        //! setup numblocks with 1-argument constructor
        template <typename U>
        inline explicit Operating(void        *workspace,
                                  const size_t numBlocks,
                                  U           &arguments) :
        Core::Operating(workspace,numBlocks,sizeof(T),Build1<U>,(void*)&arguments,Smash)
        {
            std::cerr << "building " << numBlocks << " @ItemSize=" << sizeof(T) << " with args" << std::endl;
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

