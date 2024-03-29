;
; Gilles Duvert, March 2015
;
; write a GDL or a FL file, or regenerate the IDL version
; read it back and compare with IDL reference version stored
; (file 'formats.idl')
;
; Notes on pbs found by AC, 15 May 2015:
; -- adding a positive case ...
; -- errors in FL 0.79.32 http://www.fawlty.uhostall.com/
;    just due to shift/cut in lines for formats "17"
; -- as is, at this date, this code shows the format management in GDL
;    is OK for negative input, wrong for positive one !!
; -- extra "\ ^J" added in GDL between the Re/Im parts for (D)Complex
;
; Changes: 
; 2016-01612 : AC various changes for better managing paths,
;              expecially for Cmake automatic tests !
;
; md5sum of current version of "formats.idl" (-1 et 12 ...)
; 5d1cfbc31312a833f62033e297f986a2  formats.idl
;
; ------------------------------------------------------------
;
pro INTERNAL_FORMAT_PRINTING, lun, struct
;
names=TAG_NAMES(struct)
;
form=['(b)','(o)','(z)','(i)']
for i=0, N_TAGS(struct)-1 do for j=0, N_ELEMENTS(form)-1 do $
    printf, lun, STRING(names[i],format='(a12)')+"["+form[j]+']="'+STRING(struct.(i),format=form[j])+'"'
;
form=['(b0)','(o0)','(z0)','(i0)']
for i=0, N_TAGS(struct)-1 do for j=0, N_ELEMENTS(form)-1 do $
    printf, lun, STRING(names[i],format='(a12)')+"["+form[j]+']="'+STRING(struct.(i),format=form[j])+'"'
;
form=['(b6)','(o6)','(z6)','(i6)']
for i=0, N_TAGS(struct)-1 do for j=0, N_ELEMENTS(form)-1 do $
    printf, lun, STRING(names[i],format='(a12)')+"["+form[j]+']="'+STRING(struct.(i),format=form[j])+'"'
;
form=['(b017)','(o017)','(z017)','(i017)']
for i=0, N_TAGS(struct)-1 do for j=0, N_ELEMENTS(form)-1 do $
    printf, lun, STRING(names[i],format='(a12)')+"["+form[j]+']="'+STRING(struct.(i),format=form[j])+'"'
;
form=['(b043)','(o043)','(z043)','(i043)']
for i=0, N_TAGS(struct)-1 do for j=0, N_ELEMENTS(form)-1 do $
    printf, lun, STRING(names[i],format='(a12)')+"["+form[j]+']="'+STRING(struct.(i),format=form[j])+'"'
;
form=['(b064)','(o064)','(z064)','(i064)']
for i=0, N_TAGS(struct)-1 do for j=0, N_ELEMENTS(form)-1 do $
    printf, lun, STRING(names[i],format='(a12)')+"["+form[j]+']="'+STRING(struct.(i),format=form[j])+'"'
;
end
;
; ------------------------------------------------------------
;
pro GENERATE_FORMATS_FILE, nb_cases, verbose=verbose, test=test
;
filename='formats.'+GDL_IDL_FL()
if FILE_TEST(filename) then begin
    FILE_MOVE, filename, filename+'_old', /overwrite
    MESSAGE,/cont, 'Copy of old file <<'+filename+'_old'+'>> done.'
endif
; value to be write : one negative, one positive
;
struct_neg = {BYTE:-1b,short:-1us,ushort:-1us, $
              long:-1l,ulong:-1ul,long64:-1ll, $
              ulong64:-1ull,float:-1,double:-1d, $
              cmplx:complex(-1,-1),dcmplx:dcomplex(-1,-1)}
struct_pos = {BYTE:12b,short:12us,ushort:12us, $
              long:12l,ulong:12ul,long64:12ll, $
              ulong64:12ull,float:12,double:12d, $
              cmplx:complex(12,12),dcmplx:dcomplex(12,12)}
;
GET_LUN, lun1
OPENW, lun1, filename
;
INTERNAL_FORMAT_PRINTING, lun1, struct_neg
INTERNAL_FORMAT_PRINTING, lun1, struct_pos
;
CLOSE, lun1
FREE_LUN, lun1
;
; nb_cases= nb_values * nb_formats * n_elements(struct)
nb_formats_type=6
nb_formats_formats=4
nb_cases= (N_TAGS(struct_neg)+N_TAGS(struct_pos))*nb_formats_type*nb_formats_formats
;
if KEYWORD_SET(verbose) then MESSAGE,/cont, 'File <<'+filename+'>> written.'
;
if KEYWORD_SET(test) then STOP
;
end
;
; ------------------------------------------------------------
;
; This code can be used in 3 softs : IDL, GDL, FL
; if IDL, it is used to generate the reference
;
pro TEST_FORMATS, help=help, no_exit=no_exit, test=test, debug=debug
;
ON_ERROR, 2
;
if KEYWORD_SET(help) then begin
    print, 'pro TEST_FORMATS, help=help, no_exit=no_exit, test=test, debug=debug'
    return
endif
;
; which soft do we use ??
soft=GDL_IDL_FL(/verbose)
;
GENERATE_FORMATS_FILE, nb_cases, verbose=verbose
;
; locating then read back the reference idl.xdr:
;
; we need to add the current dir. into the path because new file(s)
; are writtent in it. Do we have a simple way to check whether a dir
; is already in !PATH ?
;
CD, current=current
new_path=!path+PATH_SEP(/SEARCH_PATH)+current
list_of_dirs=STRSPLIT(new_path, PATH_SEP(/SEARCH_PATH), /EXTRACT)
;
; only this reference file is mandatory !
;
filename='formats.idl'
file_fmt_idl=FILE_SEARCH(list_of_dirs+PATH_SEP()+filename)
;
if (soft NE 'idl') AND (STRLEN(file_fmt_idl) EQ 0) then begin
    MESSAGE, 'reference file <<'+filename+'>> not found in the !PATH', /continue
    if KEYWORD_SET(no_exit) OR KEYWORD_SET(test) then STOP
    EXIT, status=1
endif
if N_ELEMENTS(file_fmt_idl) GT 1 then print, 'multiple reference file <<'+filename+'>> found !'
file_fmt_idl=file_fmt_idl[0]
;
filename='formats.gdl'
file_fmt_gdl=FILE_SEARCH(list_of_dirs+PATH_SEP()+filename)
if N_ELEMENTS(file_fmt_gdl) GT 1 then begin
    print, 'multiple reference file <<'+filename+'>> found ! First used !!'
    print, TRANSPOSE(file_fmt_gdl)
    file_fmt_gdl=file_fmt_gdl[0]
endif
;
filename='formats.fl'
file_fmt_fl=FILE_SEARCH(list_of_dirs+PATH_SEP()+filename)
if N_ELEMENTS(file_fmt_fl) GT 1 then begin
    print, 'multiple reference file <<'+filename+'>> found !'
    print, TRANSPOSE(file_fmt_fl)
    file_fmt_fl=file_fmt_fl[0]
endif
;
if (soft EQ 'idl') then begin
    soft=''
    if ~FILE_TEST(file_fmt_fl) then MESSAGE, /cont, "missing file <<formats.fl>>" else soft='fl'
    if ~FILE_TEST(file_fmt_gdl) then MESSAGE, /cont, "missing file <<formats.gdl>>" else soft='gdl'
    if (soft EQ '') then begin
        MESSAGE, /cont, "No useful file found for comparison. Just Reference file written."
        return
    endif
endif
;
; reading back the 2 files : one created ("formats.gdl" or
; "formats.fl") and one reference ("formats.idl")
;
print, "Files to be compared : formats.idl, formats."+soft
;
GET_LUN, lun1
OPENR, lun1, file_fmt_idl
GET_LUN, lun2
if (soft EQ 'gdl') then OPENR, lun2, file_fmt_gdl
if (soft EQ 'fl') then OPENR, lun2, file_fmt_fl
;
ref=STRING("")
val=STRING("")
nb_errors=0
;
;; for i=0L, 2*6*N_TAGS(struct)*N_ELEMENTS(form)-1 do begin
for i=0L, nb_cases-1 do begin
   READF, lun1, ref
   READF, lun2, val
   if KEYWORD_SET(debug) then begin
      print, ref
      print, val
   endif
   ;; for complex, print adds a supplementary blank in some cases.
   ;; try to remove it
   val=STRTRIM(val)
   if ~(STRCMP(ref,val)) then begin
      if KEYWORD_SET(test) then begin
         print, "in <<formats.idl>> : ", ref
         print, "in <<formats."+soft+">> : ", val
      endif
      nb_errors++
   endif
endfor
;
CLOSE, lun1, lun2
FREE_LUN, lun1, lun2
;
if (nb_errors GT 0) then begin
   MESSAGE, /continue, 'Using a "diff formats.idl formats.{gdl|fl}" in a shell'
   MESSAGE, /continue, 'should help to debug !'
endif
;
; ----------------- final message ----------
;
BANNER_FOR_TESTSUITE, 'TEST_FORMATS', nb_errors;, short=short
;
if (nb_errors GT 0) AND ~KEYWORD_SET(no_exit) then EXIT, status=1
;
if KEYWORD_SET(test) then STOP

end
