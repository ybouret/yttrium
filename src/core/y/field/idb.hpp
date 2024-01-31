//! \file

#ifndef Y_Field_IDB_Included
#define Y_Field_IDB_Included 1

#include "y/singleton.hpp"
#include "y/string.hpp"
#include "y/ptr/arc.hpp"

namespace Yttrium
{
    namespace Field
    {

        typedef ArcPtr<String> SharedString; //!< shared string

        //______________________________________________________________________
        //
        //
        //
        //! global database of index string
        //
        //
        //______________________________________________________________________
        class IndexDataBase : public Singleton<IndexDataBase>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const      CallSign; //!< CallSign
            static const AtExit::Longevity LifeTime = AtExit::MaximumLongevity - 15; //!< Longevity
            static const char              Prefix = '['; //!< to format
            static const char              Suffix = ']'; //!< to format

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________

            //! get create a String ready to be shared for index i
            const String & operator[](const unit_t i);

            //! empty string
            const SharedString empty;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(IndexDataBase);
            explicit IndexDataBase();
            virtual ~IndexDataBase() noexcept;
            friend class Singleton<IndexDataBase>;

        };


    }
}

#endif

