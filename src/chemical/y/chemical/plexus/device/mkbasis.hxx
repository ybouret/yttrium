{
    Y_XML_SECTION(xml, "basis");
    assert( 0 == ortho.size );
    assert( 0 == basis.size );
    assert( na>1 );

    //--------------------------------------------------------------------------
    //
    //
    // query indepedent Ansatz in promising order
    //
    //
    //--------------------------------------------------------------------------
    for(size_t i=1;i<=na;++i)
    {
        const Ansatz        & ans = ansatz[i];
        if( ortho.wouldAccept( mine.iTopo[ ans.eq.indx[SubLevel] ] ) )
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
