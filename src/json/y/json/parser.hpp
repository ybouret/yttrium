//! \file

#ifndef Y_JSON_Parser_Included
#define Y_JSON_Parser_Included 1

#include "y/singleton.hpp"
#include "y/jive/module.hpp"
#include "y/json/value.hpp"

namespace Yttrium
{

    namespace JSON
    {
        //______________________________________________________________________
        //
        //
        //
        //! Singleton Parser for JSON
        //
        //
        //______________________________________________________________________
        class Parser : public Singleton<Parser>
        {
        public:
            //__________________________________________________________________
            //
            //
            // Definitions
            //
            //__________________________________________________________________
            static const char * const      CallSign;                                 //!< "JSON"
            static const AtExit::Longevity LifeTime = AtExit::MaximumLongevity - 13; //!< longevity

            //__________________________________________________________________
            //
            //
            // Methods
            //
            //__________________________________________________________________
            void renderGraphViz() const;                   //!< render parser's architecture
            void load(JSON::Value &value, Jive::Module *); //!< load module content into value


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Parser);
            friend class Singleton<Parser>;
            
            explicit Parser();
            virtual ~Parser() noexcept;
        };
    }


}

#endif

