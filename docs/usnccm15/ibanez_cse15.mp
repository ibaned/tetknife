outputformat := "png";
outputtemplate := "%j-%c.%o";
prologues := 3;
hppp := 0.25;
vppp := 0.25;
input boxes

beginfig(1);
w:=30;
pair v[];
v0:=(0,0);
v1:=(w,0);
v2:=(2*w,0);
v3:=(0,w);
v4:=(w,w);
v5:=(2*w,w);
draw v0--v3;
draw v1--v4;
draw v2--v5;
draw v0--v1--v2;
draw v3--v4--v5;
label.bot("0",v0);
label.bot("1",v1);
label.bot("2",v2);
label.top("3",v3);
label.top("4",v4);
label.top("5",v5);
label("0",(v0+v1+v4+v3)/4);
label("1",(v1+v2+v5+v4)/4);
endfig;

beginfig(2);
boxjoin(a.ne=b.nw; a.se=b.sw);
boxit.a0("0"); boxit.a1("1");
boxit.a2("4"); boxit.a3("3");
boxit.a4("1"); boxit.a5("2");
boxit.a6("5"); boxit.a7("4");
drawboxed(a0,a1,a2,a3,a4,a5,a6,a7);
pair w;
w=a0.nw - a0.sw;
draw a0.nw--(a0.nw + w);
draw a4.nw--(a4.nw + w);
draw a7.ne--(a7.ne + w);
label.top("0",a1.ne);
label.top("1",a5.ne);
endfig;

beginfig(3);
boxjoin(b.w = a.e + (40,0))
circleit.e0("0"); circleit.e1("1");
e0.w = (30,60);
drawboxed(e0,e1);
boxjoin(b.w = a.e + (10,0))
circleit.v0("0"); circleit.v1("1");
circleit.v2("2"); circleit.v3("3");
circleit.v4("4"); circleit.v5("5");
drawboxed(v0,v1,v2,v3,v4,v5);
draw v0.n--e0.s;
draw v1.n--e0.s;
draw v4.n--e0.s;
draw v3.n--e0.s;
draw v1.n--e1.s;
draw v2.n--e1.s;
draw v5.n--e1.s;
draw v4.n--e1.s;
endfig;

beginfig(4);
boxjoin(b.w = a.e + (10,0))
circleit.v0("0"); circleit.v1("1");
circleit.v2("2"); circleit.v3("3");
circleit.v4("4"); circleit.v5("5");
drawboxed(v0,v1,v2,v3,v4,v5);
boxjoin(b.s = a.n)
boxit.u0("0");
u0.s = v0.n + (0,10);
boxjoin(b.s = a.n)
boxit.u1("0"); boxit.u2("1"); 
u1.s = v1.n + (0,10);
boxjoin(b.s = a.n)
boxit.u3("1");
u3.s = v2.n + (0,10);
boxjoin(b.s = a.n)
boxit.u4("0");
u4.s = v3.n + (0,10);
boxjoin(b.s = a.n)
boxit.u5("0"); boxit.u6("1"); 
u5.s = v4.n + (0,10);
boxjoin(b.s = a.n)
boxit.u7("1");
u7.s = v5.n + (0,10);
drawboxed(u0,u1,u2,u3,u4,u5,u6,u7);
drawarrow v0.n--u0.s;
drawarrow v1.n--u1.s;
drawarrow v2.n--u3.s;
drawarrow v3.n--u4.s;
drawarrow v4.n--u5.s;
drawarrow v5.n--u7.s;
endfig;

beginfig(5);
boxjoin(b.w = a.e + (10,0))
circleit.v0("0"); circleit.v1("1");
circleit.v2("2"); circleit.v3("3");
circleit.v4("4"); circleit.v5("5");
drawboxed(v0,v1,v2,v3,v4,v5);
defaultdx:=6bp;
boxjoin(b.w = a.e)
boxit.u0("0");
boxit.u1("0"); boxit.u2("1"); 
boxit.u3("1");
boxit.u4("0");
boxit.u5("0"); boxit.u6("1"); 
boxit.u7("1");
u0.e=(0,30);
drawboxed(u0,u1,u2,u3,u4,u5,u6,u7);
drawarrow v0.n--u0.s;
drawarrow v1.n--u1.s;
drawarrow v2.n--u3.s;
drawarrow v3.n--u4.s;
drawarrow v4.n--u5.s;
drawarrow v5.n--u7.s;
pair w;
w:=u0.n - u0.s;
draw u0.nw--(u0.nw + w);
draw u1.nw--(u1.nw + w);
draw u3.nw--(u3.nw + w);
draw u4.nw--(u4.nw + w);
draw u5.nw--(u5.nw + w);
draw u7.nw--(u7.nw + w);
draw u7.ne--(u7.ne + w);
endfig;

beginfig(6);
boxjoin(b.w = a.e + (10,0))
circleit.v0("0"); circleit.v1("1");
circleit.v2("2"); circleit.v3("3");
circleit.v4("4"); circleit.v5("5");
drawboxed(v0,v1,v2,v3,v4,v5);
boxjoin(b.w = a.e)
defaultdx:=8;
defaultdy:=6;
boxit.u0(); boxit.u1();
boxit.u2(); boxit.u3();
boxit.u4(); boxit.u5();
boxit.u6(); boxit.u7();
u0.e=(0,50);
drawboxed(u0,u1,u2,u3,u4,u5,u6,u7);
defaultdy:=4;
defaultdy:=4;
drawarrow v0.n--u0.s;
drawarrow v1.n--u1.s;
drawarrow v2.n--u5.s;
drawarrow v3.n--u3.s;
drawarrow v4.n--u2.s;
drawarrow v5.n--u6.s;
drawarrow u1.n{up}..u4.n;
drawarrow u2.n{up}..u7.n;
w:=u0.n - u0.s;
draw u0.nw--(u0.nw + w);
draw u4.nw--(u4.nw + w);
draw u7.ne--(u7.ne + w);
endfig;

beginfig(7);
defaultdx:=8bp;
boxjoin(b.w = a.e);
boxit.v0("0"); boxit.v1("1");
boxit.v2("5"); boxit.v3("3");
boxit.v4("2"); boxit.v5("6");
drawboxed(v0,v1,v2,v3,v4,v5);
defaultdx:=6bp;
boxjoin(b.w = a.e);
boxit.u0("-1"); boxit.u1("4");
boxit.u2("7"); boxit.u3("-1");
boxit.u4("-1"); boxit.u5("-1");
boxit.u6("-1"); boxit.u7("-1");
u0.e=(0,30);
drawboxed(u0,u1,u2,u3,u4,u5,u6,u7);
w:=u0.n - u0.s;
draw u0.nw--(u0.nw + w);
draw u3.ne--(u3.ne + w);
draw u7.ne--(u7.ne + w);
label.top("0",u1.ne);
label.top("1",u5.ne);
endfig;

beginfig(8);
boxit.h("1");
drawboxed(h);label.lft(btex first\_free etex,h.w);
boxjoin(a.se=b.sw; a.ne=b.nw; b.nw - b.se = a.nw - a.se);
boxit.f0("L");boxit.f1("2");boxit.f2("6");boxit.f3("L");
boxit.f4("L");boxit.f5("L");boxit.f6("-1");boxit.f7("L");
f0.n = h.s - (0,10);
drawboxed(f0,f1,f2,f3,f4,f5,f6,f7);
label.lft("free",f0.w);
drawarrow h.s--f1.n;
drawarrow f1.s{down}..f2.s;
drawarrow f2.s{down}..f6.s;
endfig;

beginfig(9);
boxjoin(b.nw = a.ne; b.sw = a.se);
boxit.a0("used"); boxit.a1("extra");
a0.e = a0.w + (80,0);
a1.e = a1.w + (40,0);
drawboxed(a0,a1);
label.top("end",a0.ne);
label.top("cap",a1.ne);
endfig;

beginfig(10);
defaultdx := 6pt;
defaultdy := 6pt;
boxjoin(b.se = a.ne; b.sw = a.nw);
boxit.t0(); boxit.t1();
drawboxed(t0,t1);
defaultdx := 2pt;
defaultdy := 2pt;
boxjoin();
boxit.a0(btex triangles to edges etex);
a0.w = t0.e + (20,00);
a0.e = a0.w + (100,0);
boxit.a1(btex quads to edges etex);
a1.w = t1.e + (20,10);
a1.e = a1.w + (120,0);
drawboxed(a0,a1);
drawarrow t0.e--a0.w;
drawarrow t1.e{right}..a1.w{right};
endfig;

end.
