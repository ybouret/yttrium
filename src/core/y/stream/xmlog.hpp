//! \file

#ifndef Y_XMLOG_Included
#define Y_XMLOG_Included 1

#include "y/string.hpp"
#include "y/ptr/auto.hpp"

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //
    //! helper to display optional XML-like log
    //
    //
    //__________________________________________________________________________
    class XMLog
    {
    public:
        //______________________________________________________________________
        //
        //
        // Defintions
        //
        //______________________________________________________________________
        static const char LANGLE = '<';     //!< alias
        static const char RANGLE = '>';     //!< alias
        static const char SLASH  = '/';     //!< alias
        static const char ENDL   = '\n';    //!< alias
        typedef AutoPtr<const String> Mark; //!< alias


        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________

        //! setup root/enter
        template <typename NAME>
        XMLog(const NAME &name, const bool flag) :
        verbose(flag),
        deep(0),
        mark( verbose ? new String(name) : 0)
        {
            enter(mark,true);
        }

        //! leave/cleanup
        virtual ~XMLog() noexcept;

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        std::ostream & operator()(void);     //!< indented
        std::ostream & operator*() noexcept; //!< raw

        //! display entering
        template <typename NAME> inline void enter(const NAME &name, const bool full) const
        {
            if(verbose)
            {
                if(full)
                    indent() << LANGLE << name << RANGLE << ENDL;
                else
                    indent() << LANGLE << name;
            }
        }

        //! display leaving
        template <typename NAME> inline void leave(const NAME &name) const {
            try
            { if(verbose) indent() << LANGLE << name << SLASH << RANGLE << ENDL; }
            catch(...) { }
        }

        //______________________________________________________________________
        //
        //
        // Members
        //
        //______________________________________________________________________
        bool           verbose;  //!< effective
        const unsigned deep;     //!< current depth
        const Mark     mark;     //!< root mark

    private:
        Y_DISABLE_COPY_AND_ASSIGN(XMLog);
        std::ostream & indent() const;

    public:

        //______________________________________________________________________
        //
        //
        //! making a local markup
        //
        //______________________________________________________________________
        class Markup
        {
        public:
            //__________________________________________________________________
            //
            //! setup
            //__________________________________________________________________
            template <typename NAME> inline
            Markup(XMLog &xml,const NAME &uid, bool full = true) :
            xml_(xml), uid_( xml.verbose ? new String(uid) : 0 )
            { incr(full); }

            void endl(); //!< end if wasn't fully started
            
            //__________________________________________________________________
            //
            //! cleanup
            //__________________________________________________________________
            ~Markup() noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Markup);
            XMLog     &xml_;
            const Mark uid_;
            void incr(bool full);
        };
    };

    //! output some optional message
#define Y_XMLOG(XML,MSG) do { if(XML.verbose) do { XML() << MSG << std::endl; } while(false); } while(false)

    //! create the xml sub name
#define Y_XML_SUB__(X,Y) X##Y

    //! instantiate the xml sub name
#define Y_XML_SUB_(HOST,ID,NAME,FLAG) Yttrium::XMLog::Markup Y_XML_SUB__(__xml,ID)(HOST,NAME,FLAG)

    //! use a local xml sub-section
#define Y_XMLSUB(HOST,NAME) Y_XML_SUB_(HOST,__LINE__,NAME,true)

    //! end a local xml sub-section header
#define Y_XMLSUB_END(ID) Y_XML_SUB__(__xml,ID).endl()

    //! make a xml sub-section with options
#define Y_XMLSUB_OPT(HOST,NAME,MSG)   \
Y_XML_SUB_(HOST,__LINE__,NAME,false); \
if(HOST.verbose)  { *HOST << MSG;  }; \
Y_XMLSUB_END(__LINE__)


}


#endif

