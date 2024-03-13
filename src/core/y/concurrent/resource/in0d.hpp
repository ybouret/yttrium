

//! \file

#ifndef Y_Concurrent_Resource0D_Included
#define Y_Concurrent_Resource0D_Included 1

#include "y/concurrent/resource/interface.hpp"


namespace Yttrium
{
    namespace Concurrent
    {

        namespace Nucleus
        {
            //__________________________________________________________________
            //
            //
            //
            //! "do-nothing" class for 0D resource
            //
            //
            //__________________________________________________________________
            class Ponctual
            {
            public:
                explicit Ponctual() noexcept; //!< setup
                virtual ~Ponctual() noexcept; //!< cleanup
                friend std::ostream & operator<<(std::ostream &, const Ponctual &); //!< display
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Ponctual);
            };
        }

        //______________________________________________________________________
        //
        //
        //
        //! Ponctual Resource for Pipeline
        //
        //
        //______________________________________________________________________
        class Resource0D : public Resource<Nucleus::Ponctual>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definition
            //
            //__________________________________________________________________
            typedef Concurrent::Resource<Nucleus::Ponctual> BaseType;   //!< alias
            typedef int                                     Type;       //!< alias, N/A
            typedef int                                     Locus;      //!< alias, N/A

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            inline void init() { this->activate(); } //!< activate
            inline void quit() { this->shutdown(); } //!< shutdown

            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            virtual ~Resource0D() noexcept; //!< cleanup
        protected:
            explicit Resource0D(const ThreadContext &) noexcept; //!< setup


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Resource0D);
        };



    }

}

#endif
