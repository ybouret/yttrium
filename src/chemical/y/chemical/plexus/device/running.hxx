
{
    Y_XML_COMMENT(xml,"Running");

    switch(na)
    {
        case 0:
            Y_DEVICE_RETURN(Jammed);

        case 1: {
            const Ansatz &ans = ansatz[1];
            mine.transfer(C,L,ans.cc, SubLevel);
        } Y_DEVICE_RETURN(Solved);

        default:
            break;
    }

    //______________________________________________________________
    //
    //
    // set common initial conditions
    //
    //______________________________________________________________
    {
        Y_XML_SECTION(xml, "InitialConditions");
        mine.transfer(Cini, SubLevel,C,L);
        ff0 = ff1 = objectiveGradient(Cini,SubLevel);
        Y_XMLOG(xml, " ff=" << Formatted::Get("%15.4g",real_t(ff0)) << " (" << ff0 << "/" << objectiveFunction(Cini,SubLevel) << ")");
        assert(ff0.mantissa>=0.0);
    }

    assert(na>=2);
    assert(ff0.mantissa>0.0);
    assert(ff1.mantissa>0.0);


    size_t good = 0;
    for(size_t i=na;i>0;--i)
    {
        Ansatz &ans = ansatz[i]; assert(Running==ans.st);
        ans.eq.mustSupport(Cini,   SubLevel);
        ans.eq.mustSupport(ans.cc, SubLevel);
        ans.ff = objectiveFunction(ans.cc,SubLevel);

        if( enhance(ans) )
        {
            ++good;
        }

    }
    Y_XML_COMMENT(xml, "good=" << good << "/" << ansatz.size() << "/" << neqs);
    HeapSort::Call(ansatz,Ansatz::IncreasingFF);
    showAnsatz(xml);
    
    // the first ansatz is the default choice
    if(trace)
    {
        Cend.ld(ansatz[1].cc); saveProfile("min.pro",1000);
    }
}
