#include "y/string.hpp"
#include "y/ptr/auto.hpp"

#include "y/utest/run.hpp"

using namespace Yttrium;

namespace Yttrium
{

    class XMLog
    {
    public:
        static const char LANGLE = '<';
        static const char RANGLE = '>';
        static const char SLASH  = '/';
        static const char ENDL   = '\n';
        typedef AutoPtr<const String> Mark;

        virtual ~XMLog() noexcept
        {
            leave(mark);
        }

        template <typename NAME>
        XMLog(const NAME &name, const bool flag) :
        verbose(flag),
        deep(0),
        mark( verbose ? new String(name) : 0)
        {
            enter(mark);
        }

        std::ostream & indent() const
        {
            for(unsigned i=deep;i>0;--i)
                std::cerr << "  ";
            return std::cerr;
        }

        std::ostream & operator()(void)
        {
            return indent();
        }

        template <typename NAME>
        void enter(const NAME &name) const
        {
            if(verbose) indent() << LANGLE << name << RANGLE << ENDL;
        }

        template <typename NAME>
        void leave(const NAME &name) const
        {
            try
            {
                if(verbose) indent() << LANGLE << name << SLASH << RANGLE << ENDL;
            }
            catch(...)
            {
            }
        }

        const bool     verbose;
        const unsigned deep;
        const Mark     mark;

        class Markup
        {
        public:
            template <typename NAME>
            inline explicit Markup(XMLog &xml,const NAME &uid) :
            xml_(xml), uid_( xml.verbose ? new String(uid) : 0 )
            {
                Coerce(xml_.deep)++;
                xml_.enter(uid_);

            }

            ~Markup() noexcept
            {
                xml_.leave(uid_);
                Coerce(xml_.deep)--;
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Markup);
            XMLog     &xml_;
            const Mark uid_;
            void incr();
            void decr() noexcept;
        };

    private:
        Y_DISABLE_COPY_AND_ASSIGN(XMLog);
    };

#define Y_XMLOG(XML,MSG) do { if(XML.verbose) do { XML() << MSG << std::endl; } while(false); } while(false)
}

Y_UTEST(io_xmlog)
{
    XMLog xml("root",false);

    Y_XMLOG(xml,"Hello");

    {
        XMLog::Markup sub(xml,"level1");
    }

}
Y_UDONE()
