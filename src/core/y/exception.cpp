
#include "y/exception.hpp"
#include "y/type/utils.hpp"
#include <iostream>
#include <cstring>
#include <cstdarg>
#include <cstdio>

#include "y/text/c-format.hpp"

namespace Yttrium
{


    Exception:: Exception() noexcept :
    std::exception(),
    story()
    {
        reset();
    }

    void Exception:: reset() noexcept
    {
        memset(story,0,sizeof(story));
    }

    Exception:: ~Exception() noexcept
    {
        reset();
    }


    void Exception:: format(const char * const fmt, void * const ptr) noexcept
    {
        assert(0!=ptr);
        C_Format::Buffer(story,sizeof(story),fmt,ptr);
        //va_list &ap = *static_cast<va_list *>(ptr);
        //vsnprintf(story, sizeof(story)-1,fmt,ap);
    }

    Exception:: Exception(const char *fmt,...) noexcept:
    std::exception(),
    story()
    {
        //reset();
        assert(0!=fmt);
        va_list ap;
        va_start(ap,fmt);
        format(fmt,&ap);
        va_end(ap);
    }

    const char * Exception:: what() const noexcept
    {
        return "Exception";
    }

    const char * Exception:: when() const noexcept
    {
        return story;
    }


    Exception:: Exception(const Exception &excp) noexcept :
    std::exception(excp),
    story()
    {
        memcpy(story, excp.story, sizeof(story) );
    }

    static inline std::ostream & indentLine(std::ostream &os, const size_t n)
    {
        for(size_t i=0;i<n;++i) os << ' ';
        return os;
    }

    static inline void displayLine(std::ostream &os, const size_t n)
    {
        for(size_t i=0;i<n;++i) os << '-';
        os << std::endl;
    }


    void Exception:: display(const size_t indent) const
    {
        const char * what_text = what();
        const size_t what_size = Core::Length(what_text);
        const char * when_text = when();
        const size_t when_size = Core::Length(when_text);
        const size_t line_size = Max(what_size,when_size)+5;
        indentLine(std::cerr,indent) << "/"; displayLine(std::cerr,line_size);
        indentLine(std::cerr,indent) << "| *** " << what_text << std::endl;
        indentLine(std::cerr,indent) << "| *** " << when_text << std::endl;
        indentLine(std::cerr,indent) << "\\"; displayLine(std::cerr,line_size);

    }

    Exception & Exception:: operator<<(const char c) noexcept
    {
        const size_t len = strlen(story);
        if(len<sizeof(story)-1) story[len] = c;
        return *this;
    }

    Exception & Exception:: operator<<(const char *msg) noexcept
    {
        if(msg)
        {
            const size_t msgLen = strlen(msg);
            size_t       length = strlen(story);
            for(size_t i=0;i<msgLen && length < sizeof(story)-1;++i,++length)
            {
                story[length] = msg[i];
            }
        }
        return *this;
    }

    Exception & Exception::add(const char *fmt, ...) noexcept
    {
        Exception excp;
        {
            va_list ap;
            va_start(ap,fmt);
            excp.format(fmt,&ap);
            va_end(ap);
        }
        return (*this) << excp.story;
    }

    Exception & Exception:: pre(const char *fmt, ...) noexcept
    {
        Exception excp;
        {
            va_list ap;
            va_start(ap,fmt);
            excp.format(fmt,&ap);
            va_end(ap);
        }
        excp << story;
        memcpy(story,excp.story, sizeof(story) );
        return (*this);
    }

}
