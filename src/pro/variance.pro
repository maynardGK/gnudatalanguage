;$Id$
;
function VARIANCE, x, double=double, NaN=NaN, dimension=dimension
;
;+
;
; NAME: VARIANCE
;
; PURPOSE: 
;     Calculates the variance of the input data
;
; CATEGORY:
;     Mathematics: Statistics
;
; CALLING SEQUENCE:
;     Result=variance(x)
;
; KEYWORD PARAMETERS: 
;     DOUBLE : Keyword for double precision calculation
;     NAN    : Flag to treat IEEE Special Floating-Point values as missing data
;     DIMENSION : if absent or equal to zero, compute the variance over the
;                 whole data. otherwise, compute along the related dimension.
;
; OUTPUTS:
;    Result is the variance of input data
;
; RESTRICTIONS:
;    The input x needs to be an array of numbers (i.e not strings,
;    struct, ptr, object)
;
; PROCEDURE:
;     variance = 1/(N-1) sum((x-mean(x))^2), 
;     Uses the MOMENT function
;
; EXAMPLE:
;     a=FINDGEN(100)
;     result=VARIANCE(a)
;     print, result
;        841.667
;
; MODIFICATION HISTORY:
;   20-Mar-2004 : Written by Christopher Lee
;   18-Jul-2005 : PC, moment.pro update
;   13-Jul-2012 : Alain Coulais : adding DIMENSION keyword, using MOMENT()
;
; LICENCE:
; Copyright (C) 2004, Christopher Lee, 2005 P. Chanial, 2012 Alain Coulais
;
; This program is free software; you can redistribute it and/or modify  
; it under the terms of the GNU General Public License as published by  
; the Free Software Foundation; either version 2 of the License, or     
; (at your option) any later version.                                   
;
;-
;
ON_ERROR, 2
;
tmp = MOMENT(x, variance=variance, double=double, NaN=NaN, $
             dimension=dimension, maxmoment=2)
;
return, variance
;
end
