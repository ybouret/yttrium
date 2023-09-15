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
        static const char LANGLE = '<';
        static const char RANGLE = '>';
        static const char SLASH  = '/';
        static const char ENDL   = '\n';
        typedef AutoPtr<const String> Mark;


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
            enter(mark);
        }

        //! leave/cleanup
        virtual ~XMLog() noexcept;

        //______________________________________________________________________
        //
        //
        // Methods
        //
        //______________________________________________________________________
        std::ostream & operator()(void);

        //! display entering
        template <typename NAME> inline void enter(const NAME &name) const
        { if(verbose) indent() << LANGLE << name << RANGLE << ENDL; }

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
        bool           verbose;
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
            Markup(XMLog &xml,const NAME &uid) :
            xml_(xml), uid_( xml.verbose ? new String(uid) : 0 )
            { incr(); }

            //__________________________________________________________________
            //
            //! cleanup
            //__________________________________________________________________
            ~Markup() noexcept;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Markup);
            XMLog     &xml_;
            const Mark uid_;
            void incr();
        };
    };

#define Y_XMLOG(XML,MSG) do { if(XML.verbose) do { XML() << MSG << std::endl; } while(false); } while(false)

    //! create the guard name
#define Y_XML_SUB__(X,Y) X##Y

    //! instantiace the guard name
#define Y_XML_SUB_(HOST,ID,NAME) volatile Yttrium::XMLog::Markup Y_XML_SUB__(__xml,ID)(HOST,NAME)

    //! use a local ScopedLock to lock HOST
#define Y_XMLSUB(HOST,NAME) Y_XML_SUB_(HOST,__LINE__,NAME)

}


#endif

