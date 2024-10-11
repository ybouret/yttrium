{
    Y_XML_SECTION(xml, "basis");
    assert( 0 == ortho.size );
    assert( 0 == basis.size );
    assert( na>1 );

    for(size_t i=1;i<=na;++i)
    {
        const Ansatz        & ans = ansatz[i];
        const Equilibrium   & eq  = ans.eq;
        const Readable<int> & nu  = mine.iTopo[ eq.indx[SubLevel] ];
        if( ortho.wouldAccept( nu ) )
        {
            switch(ortho.size)
            {
                case  0: Y_XMLOG(xml, ans << " (<--)"); break;
                default: Y_XMLOG(xml, ans); break;
            }
            ortho.expand();
            if( (basis << ans).size >= dof )
                break;
        }
    }
    assert( basisOkWith(Cini,SubLevel) );
}
