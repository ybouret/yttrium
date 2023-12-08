

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
            class Ponctual
            {
            public:
                explicit Ponctual() noexcept;
                virtual ~Ponctual() noexcept;
                friend std::ostream & operator<<(std::ostream &, const Ponctual &); //!< display
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Ponctual);
            };
        }

        class Resource0D : public Resource<Nucleus::Ponctual>
        {
        public:
            typedef Concurrent::Resource<Nucleus::Ponctual> BaseType;
            typedef int                                     Type;       //!< alias, N/A
            typedef int                                     Locus;      //!< alias, N/A

            virtual ~Resource0D() noexcept;


            inline void init() { this->activate(); }
            inline void quit() { this->shutdown(); }

        protected:
            explicit  Resource0D(const ThreadContext &) noexcept;


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Resource0D);
        };



    }

}

#endif
