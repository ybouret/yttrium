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
    //! simple class to produce XML-like logs
    //
    //
    //__________________________________________________________________________
    class XMLog
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        typedef AutoPtr<const String> Mark;
        static const char LANGLE = '<';     //!< alias
        static const char RANGLE = '>';     //!< alias
        static const char SLASH  = '/';     //!< alias

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
        XMLog(bool &globalVerbose) noexcept;
        ~XMLog() noexcept;

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        std::ostream & operator()(void);      //!< indented
        std::ostream & operator*() noexcept;  //!< raw

        static std::ostream & Indent(std::ostream &, const unsigned n);
        std::ostream &        indent(std::ostream &) const;

        //______________________________________________________________________
        //
        //
        // Members
        //
        //______________________________________________________________________
        const bool   & verbose; //!< global verbosity
        const unsigned depth;   //!< local depth

        //______________________________________________________________________
        //
        //
        //! createing a local subsection
        //
        //______________________________________________________________________
        class Section
        {
        public:
            //! setup from parent xml, a name, and full/partial display
            template <typename NAME>
            Section(XMLog       &parent,
                    const NAME  &name,
                    const bool   full) :
            xml( parent ),
            sub( xml.verbose ? new String(name) : 0 )
            {
                enter(full);
            }

            //! leave/cleanup
            ~Section() noexcept;


            XMLog      &xml;
            const Mark sub;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Section);
            void enter(const bool);

        };

    private:
        Y_DISABLE_COPY_AND_ASSIGN(XMLog);
    };

    //! output some optional message
#define Y_XMLOG(XML,MSG) do { if(XML.verbose) do { XML() << MSG << std::endl; } while(false); } while(false)

    //! create the xml sub name
#define Y_XML_SUB__(X,Y) X##Y

    //! instantiate the xml sub name
#define Y_XML_SUB_(HOST,ID,NAME,FLAG) volatile Yttrium::XMLog::Section Y_XML_SUB__(__xml,ID)(HOST,NAME,FLAG)

    //! use a local xml sub-section
#define Y_XML_SECTION(HOST,NAME) Y_XML_SUB_(HOST,__LINE__,NAME,true)


    //! make a xml sub-section with options
#define Y_XML_SECTION_OPT(HOST,NAME,OPTIONS)   \
Y_XML_SUB_(HOST,__LINE__,NAME,false);          \
do if(HOST.verbose)  { (*HOST << OPTIONS) <<  Yttrium::XMLog::RANGLE << std::endl; } while(false)



}


#endif

