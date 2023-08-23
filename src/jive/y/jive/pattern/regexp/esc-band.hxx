
inline Pattern * escBand()
{
    if(curr>=last) throw Specific::Exception(CallSign,"unfinished escape sequence in '%s'",expr);

    const uint8_t esc = *(curr++);

    switch(esc)
    {
            Y_ESC_ECHO(BACKSLASH);
            Y_ESC_ECHO(QUOTE);
            Y_ESC_ECHO(DQUOTE);
            Y_ESC_ECHO(LBRACK);
            Y_ESC_ECHO(RBRACK);
            Y_ESC_ECHO(MINUS);
            Y_ESC_CTRL();

        case 'x':
            return escHexa();


        default:
            break;
    }

    throw Specific::Exception(CallSign,"unknow escape character '%s", ASCII::Printable::Char[esc]);
}
