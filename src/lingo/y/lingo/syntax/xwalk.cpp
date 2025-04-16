
#include "y/lingo/syntax/xwalk.hpp"
#include "y/lingo/syntax/xlist.hpp"
#include "y/type/nullify.hpp"

#include "y/associative/suffix/map.hpp"
#include "y/system/exception.hpp"

namespace Yttrium
{
    namespace Lingo
    {
        namespace Syntax
        {

            class XWalk:: Code : public Quantized
            {
            public:
                inline explicit Code() : db()
                {
                }

                inline virtual ~Code() noexcept {}

                inline Result process(XNode * const node,
                                      const size_t  deep) const
                {
                    assert(0!=node);

                    switch(node->type)
                    {
                        case XNode::Internal: Core::Indent(std::cerr,deep) << node->name()   << std::endl; break;
                        case XNode::Terminal: Core::Indent(std::cerr,deep) << node->lexeme() << std::endl; break;
                    }

                    const Proc * const pp = db.search(node->name());
                    if(!pp) return Dive;
                    return Coerce(*pp)(*node,deep);
                }


                SuffixMap<String,Proc> db;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Code);
            };



            XWalk:: XWalk() : code( new Code() )
            {

            }

            XWalk:: ~XWalk() noexcept
            {
                assert(0!=code);
                Nullify(code);
            }

            void XWalk:: operator()(XNode &root) const
            {
                walk(&root,0);
            }


            void XWalk:: record(const String &key, const Proc &proc)
            {
                assert(0!=code);
                if(!code->db.insert(key,proc))
                    throw Specific::Exception("Syntax::XWalk", "record multiple '%s'", key.c_str());
            }

            void XWalk:: record(const char * const key, const Proc &proc)
            {
                const String _(key);
                record(_,proc);
            }


            void XWalk:: walk(XNode * const node,
                              size_t        deep) const
            {
                assert(0!=node);

                // process current
                const Result result = code->process(node,deep);

                // intercept terminal
                switch(node->type)
                {
                    case XNode::Terminal:  return; // result has no effect
                    case XNode::Internal:  break;
                }

                // intercept result
                switch(result)
                {
                    case Stop: return;
                    case Dive: break;

                }
                
                ++deep;
                XList &list = node->branch();
                for(XNode *curr=list.head;curr;curr=curr->next)
                {
                    walk(curr,deep);
                }

            }
        }
    }

}
