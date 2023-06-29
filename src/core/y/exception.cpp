
#include "y/exception.hpp"
#include "y/type/utils.hpp"
#include <iostream>
#include <cstring>
#include <cstdarg>
#include <cstdio>

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


    void Exception:: format(const char *fmt, void *ptr) noexcept
    {
        assert(0!=ptr);
        va_list &ap = *static_cast<va_list *>(ptr);
        vsnprintf(story, sizeof(story)-1,fmt,ap);
    }

    Exception:: Exception(const char *fmt,...) noexcept:
    std::exception(),
    story()
    {
        reset();
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

    static inline
    void displayLine(const size_t n)
    {
        for(size_t i=0;i<n;++i) std::cerr << '-';
        std::cerr << std::endl;
    }

    void Exception:: display() const
    {
        const char * what_text = what();
        const size_t what_size = LengthOf(what_text);
        const char * when_text = when();
        const size_t when_size = LengthOf(when_text);
        const size_t line_size = Max(what_size,when_size)+5;
        std::cerr << "/"; displayLine(line_size);
        std::cerr << "| *** " << what_text << std::endl;
        std::cerr << "| *** " << when_text << std::endl;
        std::cerr << "\\"; displayLine(line_size);

    }


}
