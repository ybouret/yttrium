//! \file

#ifndef Y_XMLOG_Included
#define Y_XMLOG_Included 1

#include "y/string.hpp"
#include "y/ptr/auto.hpp"

namespace Yttrium
{


    //! output some optional message
#define Y_XMLOG(XML,MSG) do { if((XML).verbose) do { (XML)() << MSG << std::endl; } while(false); } while(false)

    //! create the xml sub name
#define Y_XML_SUB__(X,Y) X##Y

    //! instantiate the xml sub name
#define Y_XML_SUB_(HOST,ID,NAME,FLAG) volatile Yttrium::XMLog::Section Y_XML_SUB__(__xml,ID)(HOST,NAME,FLAG)

    //! use a local xml sub-section
#define Y_XML_SECTION(HOST,NAME) Y_XML_SUB_(HOST,__LINE__,NAME,true)


    //! make a xml sub-section with options
#define Y_XML_SECTION_OPT(HOST,NAME,OPTIONS)   \
Y_XML_SUB_(HOST,__LINE__,NAME,false);          \
do if( (HOST).verbose)  { (*HOST << OPTIONS) <<  Yttrium::XMLog::RANGLE << std::endl; } while(false)

    //! display a named list
#define Y_XML_LIST(HOST,NAME) do { HOST.display( #NAME, NAME); } while(false)

    //__________________________________________________________________________
    //
    //
    //
    //! simple class to produce XML-like logs
    //
    //
    //__________________________________________________________________________
    class XMLog : public Object, public Counted
    {
    public:
        //______________________________________________________________________
        //
        //
        // Definitions
        //
        //______________________________________________________________________
        typedef AutoPtr<const String> Mark; //!< alias
        static const char LANGLE = '<';     //!< alias
        static const char RANGLE = '>';     //!< alias
        static const char SLASH  = '/';     //!< alias

        //______________________________________________________________________
        //
        //
        // C++
        //
        //______________________________________________________________________
        XMLog(bool &globalVerbose) noexcept; //!< setup with global verbosity ref
        ~XMLog()                   noexcept; //!< cleanup

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        std::ostream & operator()(void);      //!< indented
        std::ostream & operator*() noexcept;  //!< raw

        static std::ostream & Indent(std::ostream &, const unsigned n); //!< indent n times
        std::ostream &        indent(std::ostream &) const;             //!< indent depth


        template <typename NAME, typename LIST> inline
        void display(const NAME &name, const LIST &list)
        {
            const size_t n = list.size;
            if(n<=0)
            {
                empty(name);
            }
            else
            {
                Y_XML_SECTION_OPT(*this,name, " size='" << n << "'");
                for(const typename LIST::NodeType *node=list.head;node;node=node->next)
                {
                    Y_XMLOG(*this,**node);
                }
            }
        }

        template <typename NAME> inline
        void empty(const NAME &name)
        {
            if(verbose) {
                (*this)() << LANGLE << name << SLASH << RANGLE << std::endl;
            }
        }


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


            XMLog      &xml; //!< output
            const Mark sub;  //!< sub-section name

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Section);
            void enter(const bool);

        };

    private:
        Y_DISABLE_COPY_AND_ASSIGN(XMLog);
    };


}


#endif

