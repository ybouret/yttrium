
#define Y_ESC_ECHO(CH) case CH: return new Single(CH)

#define Y_ESC_CTRL()               \
case 'a': return new Single('\a'); \
case 'b': return new Single('\b'); \
case 'e': return new Single('\e'); \
case 'f': return new Single('\f'); \
case 'n': return new Single('\n'); \
case 'r': return new Single('\r'); \
case 't': return new Single('\t'); \
case 'v': return new Single('\v')

inline Pattern * escExpr()
{
    if(curr>=last) throw Specific::Exception(CallSign,"unfinished escape sequence in '%s'",expr);

    const uint8_t esc = *(curr++);

    switch(esc)
    {
            Y_ESC_ECHO(LPAREN);
            Y_ESC_ECHO(RPAREN);
            Y_ESC_ECHO(ALT);
            Y_ESC_ECHO(BACKSLASH);
            Y_ESC_ECHO(QUOTE);
            Y_ESC_ECHO(DQUOTE);
            Y_ESC_ECHO(LBRACK);
            Y_ESC_ECHO(RBRACK);
            Y_ESC_ECHO(LBRACE);
            Y_ESC_ECHO(RBRACE);
            Y_ESC_ECHO('+');
            Y_ESC_ECHO('*');
            Y_ESC_ECHO('?');
            Y_ESC_ECHO('.');
            Y_ESC_CTRL();


        case 'x':
            return escHexa();
       

        default:
            break;
    }

    throw Specific::Exception(CallSign,"unknown escaped character '%s", ASCII::Printable::Char[esc]);
}
