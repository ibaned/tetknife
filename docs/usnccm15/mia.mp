outputformat := "png";
outputtemplate := "%j.%o";
prologues := 3;
hppp := 0.25;
vppp := 0.25;
input boxes

beginfig(1);
boxjoin(a.se = b.ne; a.sw = b.nw);
boxit.t0();
boxit.t1();
boxit.t2();
boxjoin();
boxit.fu(btex first triangle use $(n_t)$ etex scaled 0.6);
fu.w = t0.e + (10,0);
fu.e = fu.w + (60,0);
t0.n - t0.s = fu.n - fu.s + (0,5);
boxjoin();
boxit.eu(btex edges used $(3n_t)$ etex scaled 0.6);
t1.n - t1.s = eu.n - eu.s + (0,5);
eu.w = t1.e + (10,0);
eu.e = eu.w + (100,0);
boxjoin();
boxit.neu(btex next edge use $(3n_t)$ etex scaled 0.6);
neu.w = t2.e + (10,0);
neu.e = neu.w + (100,0);
t2.n - t2.s = neu.n - neu.s + (0,5);
boxjoin();
boxit.efu(btex first edge use $(n_e)$ etex scaled 0.6);
boxjoin();
boxit.e0();
e0.ne = t2.se - (0,10);
e0.nw = t2.sw - (0,10);
efu.w = e0.e + (10,0);
efu.e = efu.w + (60,0);
e0.n - e0.s = efu.n - efu.s + (0,5);
drawboxed(fu, eu, neu, efu);
drawboxed(t0,t1,t2,e0);
drawarrow t0.e--fu.w;
drawarrow t1.e--eu.w;
drawarrow t2.e--neu.w;
drawarrow e0.e--efu.w;
endfig;

end.
