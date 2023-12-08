
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
            typedef void (*Build)(void *, void *);       //!< constructor prototype
            typedef void (*XCopy)(void *, const void *); //!< copy prototype
            typedef void (*Smash)(void *);               //!< destructor  prototype

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
        protected:
            //! setup with build
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

            //! setup with copy
            explicit Operating(void            *blockAddr,
                               const size_t     blockSize,
                               const Operating &source,
                               XCopy            xcopy,
                               Smash            smash);

        public:
            virtual ~Operating() noexcept; //!< cleanup, release all

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            const void *entry()  const noexcept; //!< recall entry point
            size_t      blocks() const noexcept; //!< recall built blocks

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
        Core::Operating(workspace,numBlocks,sizeof(T),SelfBuild,0,SelfSmash)
        {
        }

        //! setup numblocks with 1-argument constructor
        template <typename U>
        inline explicit Operating(void        *workspace,
                                  const size_t numBlocks,
                                  U           &arguments) :
        Core::Operating(workspace,numBlocks,sizeof(T),SelfBuild1<U>,(void*)&arguments,SelfSmash)
        {
        }

        //! copy objects with [copy|manual]
        template <typename U>
        inline explicit Operating(void *target, const Operating<U> &source) :
        Core::Operating(target,sizeof(T),source,XCopyAny<U>,SelfSmash)
        {
        }


        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        inline Type      * operator()(void)       noexcept { return (Type *     )(entry()); } //!< recall access
        inline ConstType * operator()(void) const noexcept { return (ConstType *)(entry()); } //!< recall access, const


    private:
        Y_DISABLE_COPY_AND_ASSIGN(Operating);

        static inline void SelfBuild(void *addr, void *)
        {
            assert(0!=addr);
            new (addr) MutableType();
        }

        static inline void SelfSmash(void *addr) noexcept
        {
            assert(0!=addr);
            Destruct(static_cast<MutableType *>(addr));
        }

        template <typename U>
        static inline void SelfBuild1(void *addr, void *args)
        {
            assert(0!=addr);
            assert(0!=args);
            new (addr) MutableType( *static_cast<U*>(args) );
        }

        template <typename U>
        static inline void XCopyAny(void *addr, const void *from)
        {
            assert(0!=addr);
            assert(0!=from);
            new (addr) MutableType( *static_cast<const typename TypeTraits<U>::MutableType *>(from) );
        }
        
    };
}

#endif

