;; test some language elements of GDL

pro if_test

if 1 then $
  if 0 then begin & end $
else if 1 then $ 
  if 0 then begin & end $
else a=1

if a ne 1 then print,'***IF: ERROR'

print,'IF: OK'
end


pro switch_test

a='abc'

;; empty
switch a of

    1:
    complex(1): begin & end
    'def':
    'abc':
    1.0: begin & end
    else:
endswitch

switch a of

    1: print,'***SWITCH: ERROR'
    complex(1): print,'***SWITCH: ERROR'
    'def': print,'***SWITCH: ERROR'
    'abc': begin
        abc = 1
    end
    1.0: fall = abc+1
    else: e=1
endswitch

if abc ne 1 then print,'***CASE: ERROR'
if fall ne abc+1 then print,'***CASE: ERROR'
if e ne 1 then print,'***CASE: ERROR'

print,'SWITCH: OK'
end

pro case_test

a='abc'

case a of

    1: print,'***CASE: ERROR'
    complex(1): print,'***CASE: ERROR'
    'def': print,'***CASE: ERROR'
    'abc': begin
        c=1
    end
    1.0: print,'***CASE: ERROR'
    else: print,'***CASE: ERROR'
endcase

if c ne 1 then print,'***CASE: ERROR'

case a of

    1: print,'***CASE: ERROR'
    complex(1): print,'***CASE: ERROR'
    'def': print,'***CASE: ERROR'
    'a': begin
        print,'***CASE: ERROR'
    end
    1.0: print,'***CASE: ERROR'
    else: e=1
end

if e ne 1 then print,'***CASE: ERROR'

print,'CASE: OK'
end

pro while_test

i=0
while i lt 100 do begin
i=i+1
endwhile

if i eq 100 then print,'WHILE: OK' else print,'***WHILE: ERROR'
end

pro for_test

c=0
for i=0,99 do c=c+1 

if c eq 100 then print,'FOR: OK' else print,'***FOR: ERROR'

c=0
for i=0,990,10 do c=c+1 

if c eq 100 then print,'FOR: Step: OK' else print,'***FOR: Step: ERROR'
end

pro repeat_test

i=0
repeat begin 
i=i+1
endrep until (i eq 100)

repeat begin 
endrep until 1

if i eq 100 then print,'REPEAT: OK' else print,'***REPEAT: ERROR'
end

pro goto_test

;; simple
goto,jump11
print,'***GOTO: Error'
jump21:
goto,jump31
print,'***GOTO: Error'
jump11:
goto,jump21
print,'***GOTO: Error'
jump31:


count=0
;; within loop
for i=0,10 do begin

goto,jump1
print,'***GOTO: Error1'
jump2:
goto,jump3
print,'***GOTO: Error2'
jump1:
goto,jump2
print,'***GOTO: Error3'
jump3:
goto,jump4 ; even to jump out of the block and in again is ok
jump5:

if 0 then goto,jump5 else goto,jump7
print,'***GOTO: Error4'

jump7:

if 1 then begin

    goto,jump8
    print,'***GOTO: Error5'
jump8:

endif else begin

    print,'***GOTO: Error6'
endelse


count=count+1
endfor

goto,jump6
jump4:
goto,jump5
jump6:

if count ne 11 then print,'***GOTO: ERROR7'

print,'GOTO: OK'

end

function ret99
return,99b
end

function ret,a
return,a
end

function retEq,a
return,(a=3)
end

function retTag,a
return,(a.t=3)
end

function ret2,b
return,ret(b)
end

function ret3,b
return,ret2(b)
end

pro ret_test

if ret99() ne 99.0 then print,'***RET: ERROR1'
if retEq(u) ne 3 then print,'***RET: ERROR2'
(retEq(u2)) = 4
if u2 ne 4 then print,'***RET: ERROR4'


(ret3(u3))=7
if u3 ne 7 then print,'***RET: ERROR5'

s={t:0}
if retTag(s) ne 3 then print,'***RET: ERROR6'
;(ret(s))=2

print,'RET: OK';
end


pro struct_test

s={tag: indgen(5)}

if s.tag[ 3] ne 3 then print,'***STRUCT: ERROR1'

s.tag++

if s.tag[ 3] ne 4 then print,'***STRUCT: ERROR2'

s.tag = 9

if s.tag[ 3] ne 9 then print,'***STRUCT: ERROR3'

s.tag = [10,11,12]

if s.tag[ 2] ne 12 then print,'***STRUCT: ERROR4'

ss=[s,s]

ss[*].tag=indgen(5,2)

if ss[1].tag[2] ne 7 then print,'***STRUCT: ERROR5'

s={s:indgen(5)}
ss={ss:s}
sss={sss:ss}

sss.sss.ss.s[2] = 10
if sss.sss.ss.s[2] ne 10 then print,'***STRUCT: ERROR6'

(ret(sss)).sss.ss.s[1] = 11
if sss.sss.ss.s[1] ne 11 then print,'***STRUCT: ERROR7'
if (ret(sss)).sss.ss.s[3] ne 3 then print,'***STRUCT: ERROR8'

(ret(sss)).sss.(0).s[1] = 11
if sss.(0).ss.s[1] ne 11 then print,'***STRUCT: ERROR9'

if ({a:3}).a ne 3 then  print,'***STRUCT: ERROR10'

;; array and struct
s4=[sss,sss]
s5=[[s4],[s4]]
if s5[1,1].sss.ss.s[3] ne 3 then print,'***STRUCT: ERROR11'

print,'STRUCT: OK'
end

pro multi,a,b,c
b=9
if a ne 9 then print,'***MULTI: ERROR1'
if c ne 9 then print,'***MULTI: ERROR2'
end

pro multi_test
multi,a,a,a
if a ne 9 then print,'***MULTI: ERROR3'
print,'MULTI: OK'
end

function o::init
self.a=9
return,1
end

function o::get
return,self.a
end

pro o::test
self.a=1
if self.a ne 1 then print,'***OBJECT: ERROR1'
self[0].a=2
if self[0].a ne 2 then print,'***OBJECT: ERROR2'
(self).a[0]=3
if (self[0]).a[[-2]] ne 3 then print,'***OBJECT: ERROR3'
if self[[0]].a ne 3 then print,'***OBJECT: ERROR4'
;; would be legal in GDL (but IDL complains)
;if (self[[0]]).a ne 3 then print,'***OBJECT: ERROR5'
end

pro o::cleanup
common object_test,o_cleanup
o_cleanup=-1
end

pro object_test

common object_test

;; object
s={o,a:0}
obj=obj_new('o')

if obj->get() ne 9 then print,'***OBJECT: ERROR6'

obj->test
(obj)->test
obj[0]->test
(obj[0])->test

if not obj_valid( obj) then print,'***OBJECT: ERROR7'

obj_destroy,obj

if obj_valid( obj) then print,'***OBJECT: ERROR8'
if o_cleanup ne -1 then print,'***OBJECT: ERROR9'

print,'OBJECT: OK'
end

pro ct1
common tc,x,y,z
if x ne 3 then print,'***COMMON: ERROR1'
y=7
end

pro ct2
common tc
if x ne 3 then print,'***COMMON: ERROR2'
if y ne 7 then print,'***COMMON: ERROR3'
z=9
end

function ct3
common tc3,a
return,a
end

pro common_test
common tc,a,b,c

a=3
ct1
ct2

if a ne 3 then print,'***COMMON: ERROR4'
if b ne 7 then print,'***COMMON: ERROR5'
if c ne 9 then print,'***COMMON: ERROR6'

(ct3()) = 2
if ct3() ne 2 then print,'***COMMON: ERROR7'

print,'COMMON: OK'
end

pro set22,a
a=2
end

pro ref_test

set22,b
if b ne 2 then print,"***REF: ERROR1"

p=ptr_new(/alloc)
set22,*p
if *p ne 2 then print,"***REF: ERROR2"
ptr_free,p

p=ptr_new(/alloc)
pp=ptr_new(p)
set22,**pp
if **pp ne 2 then print,"***REF: ERROR3"
ptr_free,p,pp

print,'REF: OK'
end

function base::init
common inheritance,baseInit,baseCleanup,derivInit,derivCleanup
baseInit=1
return,1
end
pro base::cleanup
common inheritance,baseInit,baseCleanup,derivInit,derivCleanup
baseCleanup=1
end
pro base::set
self.b='base'
end
pro base::setbase
self.b='base'
end
function base::get
return,self.b
end

function deriv::init
common inheritance,baseInit,baseCleanup,derivInit,derivCleanup
r=self->base::init()
derivInit=1
return,1
end
pro deriv::cleanup
common inheritance,baseInit,baseCleanup,derivInit,derivCleanup
self->base::cleanup
derivCleanup=1
end
pro deriv::set
self.b='deriv'
end

pro inheritance_test

base={ base, b: ''}
deriv={ deriv, INHERITS base, d: ''}

common inheritance

o=obj_new( 'deriv')
if baseInit ne 1 then print,'***INHERITANCE: ERROR1'
if derivInit ne 1 then print,'***INHERITANCE: ERROR2'

o->setbase
if o->get() ne 'base' then print,'***INHERITANCE: ERROR3'
o->set
if o->get() ne 'deriv' then print,'***INHERITANCE: ERROR4'
o->base::set
if o->get() ne 'base' then print,'***INHERITANCE: ERROR5'

obj_destroy, o
if baseInit ne 1 then print,'***INHERITANCE: ERROR6'
if derivInit ne 1 then print,'***INHERITANCE: ERROR7'

print,'INHERITANCE: OK'
end

pro syntax_test

s={a:0}

t=s[0].a
t=s[[0]].a
t=(s[0]).a
t=(s[[0]]).a

(a=2)
;((a=2))
;(a=2)=3
(a=2)++
((a=2))++
end

pro continuebreak_test

repeat begin

    goto,start

    print,'***CONTINUE/BREAK: ERROR2'

start:

    for i=0,40,2 do begin

        if i lt 6 then continue

        if i lt 6 then print,'***CONTINUE/BREAK: ERROR1'

        if i gt 10 then break

        if i gt 10 then print,'***CONTINUE/BREAK: ERROR2'

        goto,in1
        print,'***CONTINUE/BREAK: ERROR2'
        goto,out1
in1:

    endfor

goto,out2
out1:
out2:

endrep until 1

print,'CONTINUE/BREAK: OK'
end


pro e2,A=a,B=b
if a ne 'a' then print,'EXTRA: ERROR1'
if b ne 'b' then print,'EXTRA: ERROR2'
end

pro eref,_REF_EXTRA=ex
e2,_EXTRA=ex
end

pro eval,_EXTRA=ex
e2,_EXTRA=ex
end

pro e1,R2=a,R1=b
a=1
b=2
end

pro eret,_REF_EXTRA=ex
e1,_EXTRA=ex
end


pro extra_test,_REF_EXTRA=ex

eval,b='b',a='a'
eref,b='b',a='a'

eret,r1=r1,r2=r2

if r1 ne 2 then print,'***EXTRA: ERROR3'
if r2 ne 1 then print,'***EXTRA: ERROR4'

print,'EXTRA: OK'
end

pro expr_test

a=0
a++
a *= 2
a ^= 2
b = a+2*3-(9*2)+9
if b ne 1 then print,'***EXPR: ERROR1'

a=indgen(6,6,6)
a[3,3,3]=-1
if a[3,3,3] ne -1 then print,'***EXPR: ERROR2'

a=a+100
if a[3,3,3] ne 99 then print,'***EXPR: ERROR2a'

a[*]=a[*]-100
if a[3,3,3] ne -1 then print,'***EXPR: ERROR2b'


sdef={s,a:intarr(3),s:strarr(3)}
s1={s,indgen(3),s:sindgen(3)}
if s1.a[1] ne 1 then print,'***EXPR: ERROR3'
s2={s}
sArr=[s2,s2]
sArr[1].s[1]='test'
if sArr[1].s[1] ne 'test' then print,'***EXPR: ERROR4'

print,'EXPR: OK'
end

function fac,n

if n le 1 then return,1

return,fac(n-1)*n

end

pro recursion_test

if fac( 5) ne 120 then print,"***RECURSION: ERROR"

print,'RECURSION: OK'
end

pro index_test

b=byte( "abcdefg")
d=bytarr(20)
d[10]=b

if string( d[10:16]) ne "abcdefg" then  print,"***INDEX: ERROR1"

i=indgen(3,3)
s=[[-1],[-2]]

i[1,1]=s

if i[1,2] ne -2 then  print,"***INDEX: ERROR2"

print,'ARRAY INDEXING: OK'
end

pro operator_test

a=indgen(4)
b=indgen(4)

c=a#b

if c[2,3] ne 6 then print,"***OPERATOR: ERROR1"

b=indgen(4,2)

c=a#b

if c[0,1] ne 38 then print,"***OPERATOR: ERROR2"

a=indgen(2,4)
b=indgen(4)

c=a#b

if c[1] ne 34 then print,"***OPERATOR: ERROR3"

b=indgen(4,2)

c=a#b

if c[0,1] ne 76 then print,"***OPERATOR: ERROR4"

print,'OPERATORS: OK'
end

pro test_suite

switch_test
case_test
while_test
for_test
repeat_test
goto_test
ret_test
struct_test
multi_test
object_test
common_test
ref_test
syntax_test
inheritance_test
continuebreak_test
extra_test
expr_test
recursion_test
index_test
operator_test

end
