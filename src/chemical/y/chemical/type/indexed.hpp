
//! \file


#ifndef Y_Chemical_Indexed_Included
#define Y_Chemical_Indexed_Included 1

#include "y/chemical/type/entity.hpp"

namespace Yttrium
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //
        //! Index Level
        //
        //
        //______________________________________________________________________
        enum Level
        {
            TopLevel, //!< global system
            SubLevel, //!< sub systems
            AuxLevel  //!< aux systems
        };

        //______________________________________________________________________
        //
        //
        //
        //! Base class for indexed entities
        //
        //
        //______________________________________________________________________
        class Indexed : public Entity
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const size_t MaxLevel = AuxLevel+1; //!< number of description levels


            //__________________________________________________________________
            //
            //
            // C++
            //
            //__________________________________________________________________
            explicit Indexed(const String *, const size_t i) noexcept; //!< setup
            virtual ~Indexed() noexcept;                               //!< cleanup

            //! access
            template <typename T> inline
            T & operator()(Writable<T> &arr, const Level level) const
            {
                return arr[ indx[level] ];
            }

            //! access
            template <typename T> inline
            const T & operator()(const Readable<T> &arr, const Level level) const
            {
                return arr[ indx[level] ];
            }

            //__________________________________________________________________
            //
            //
            // Members
            //
            //__________________________________________________________________
            const size_t indx[MaxLevel]; //!< assigned indices
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Indexed);
            void setup(const size_t i) noexcept;
        };

    }

}

#endif

