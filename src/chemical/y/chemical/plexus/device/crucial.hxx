
{
    size_t makeSafer = 0;
    while(true)
    {

        //__________________________________________________________
        //
        //
        // detect situation
        //
        //__________________________________________________________
        ++makeSafer;
        Y_XML_COMMENT(xml, "makeSafer #" << makeSafer);
        ansatz.free();
        bool crucial = false;
        for(const ENode *en=mine.head;en;en=en->next)
        {
            const size_t       ii = 1+ansatz.size();
            const Equilibrium &eq = **en;
            const xreal_t      ek = K[eq.indx[TopLevel]];
            XWritable         &cc = mine.transfer(EqConc[ii],SubLevel,C, L);
            XWritable         &dc = EqDiff[ii];
            const Situation    st = aftermath.seek(cc,SubLevel,eq,ek);

            switch(st)
            {
                case Blocked: Y_XMLOG(xml, "[Blocked] " << eq);
                    continue;

                case Crucial: Y_XMLOG(xml, "[Crucial] " << eq);
                    crucial = true;
                    break;

                case Running: if(crucial) continue; // won't store any more running
                    Y_XMLOG(xml, "[Running] " << eq);
                    break;
            }

            const xreal_t xi = aftermath.eval(dc, cc, SubLevel, C, L, eq);
            const Ansatz  ans(eq,ek,st,cc,xi,dc);
            ansatz << ans;
        }
        if(!crucial) break; // will process all Running

        //__________________________________________________________
        //
        //
        // remove running among crucial
        //
        //__________________________________________________________
        assert( ansatz.size() > 0 );
        for(size_t i = ansatz.size(); i>0;--i )
        {
            Ansatz &ans = ansatz[i];
            switch(ans.st)
            {
                case Blocked: // shouldn't get here
                    throw Specific::Exception(fn, "forbidden Blocked %s", ans.eq.name.c_str());

                case Crucial: // keep
                    continue;

                case Running: // remove
                    ansatz.remove(i);
                    continue;
            }
        }

        assert(ansatz.size()>0); // must have at least one Crucial

        //__________________________________________________________
        //
        //
        // order by decreasing |xi|
        //
        //__________________________________________________________
        HeapSort::Call(ansatz, Ansatz::DecreasingAX);
        Y_XML_COMMENT(xml, "[Crucial]");
        showAnsatz(xml);

        //__________________________________________________________
        //
        //
        // set new starting phase space and check again
        //
        //__________________________________________________________
        {
            const Ansatz &ans = ansatz.head();
            mine.transfer(C, L, ans.cc, SubLevel);
        }
    }
    }
