#include "y/jive/source.hpp"
#include "y/system/exception.hpp"
#include "y/ptr/auto.hpp"
#include "y/type/nullify.hpp"
#include <cstring>

namespace Yttrium
{
    namespace Jive
    {

        class Source::Code : public Object
        {
        public:
            inline explicit Code(Module *h) noexcept :
            Object(), handle(h), cache()
            {
            }

            virtual ~Code() noexcept {}

            inline Char *get() { return cache.size>0 ? cache.popHead() : handle->get(); }

            inline bool ready()
            {
                if(cache.size>0)
                    return true;

                Char *ch = handle->get();
                if(!ch)
                    return false;
                
                cache.pushHead(ch);
                return true;
            }



            AutoPtr<Module> handle;
            Token           cache;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Code);
        };



        Source:: Source(Module *h) :
        code(0)
        {
            if(0==h) throw Specific::Exception("Jive::Source","NULL module");
            AutoPtr<Module> guard(h);
            code = new Code(h);
            (void) guard.yield();
        }


        Source:: ~Source() noexcept
        {
            assert(0!=code);
            Nullify(code);
        }


        Char * Source:: get()
        {
            assert(0!=code);
            return code->get();
        }

        void Source:: put(Char *chr) noexcept
        {
            assert(0!=chr);
            assert(0!=code);
            code->cache.pushHead(chr);
        }

        void Source:: put(Token &token) noexcept
        {
            assert(0!=code);
            code->cache.mergeHead(token);
        }

        bool  Source:: ready()
        {
            assert(0!=code);
            return code->ready();
        }

        void Source:: skip() noexcept
        {
            assert(code!=0);
            assert(code->cache.size>0);
            delete code->cache.popHead();
        }


        void Source:: dup(const Token &token)
        {
            Token cpy(token);
            put(cpy);
        }


        void  Source:: newLine() noexcept
        {
            assert(0!=code);
            code->handle->newLine();
        }



        const Module * Source:: operator->() const noexcept
        {
            assert(0!=code);
            assert(code->handle.isValid());
            return & *(code->handle);
        }


        static inline bool isBlank(const Char *ch) noexcept
        {
            assert(0!=ch);
            static const char sep[] = " \t\r\n";
            return 0 != strchr(sep,**ch);
        }


        bool Source:: guess(Token &token)
        {
            token.release();
            while(true)
            {
                Char *ch = get();
                if(!ch) break;
                if(isBlank(ch))
                {
                    delete ch;
                    continue;
                }
                else
                {
                    token.pushTail(ch);
                    break;
                }
            }

            while(true)
            {
                Char *ch = get();
                if(!ch) break;
                if(isBlank(ch))
                {
                    put(ch);
                    break;
                }
                else
                {
                    token.pushTail(ch);
                    continue;
                }
            }

            
            return token.size > 0;
        }

    }

}
