;+
; NAME:   XMANAGER
;
; PURPOSE:  Provides event management for GDL widgets.
;
; CATEGORY: GUI, widgets
;
; CALLING SEQUENCE:
;
; XMANAGER, NAME, ID, /NO_BLOCK, GROUP_LEADER=groupLeader, CLEANUP=Cleanup
;
;
; inputs:
;
; OPTIONAL INPUTS:  none
;
; KEYWORD PARAMETERS:
;
; COMMON BLOCKS:    none
;
; SIDE EFFECTS:     none
;
; RESTRICTIONS:     incomplete, more functionality to be added.
;
;
; MODIFICATION HISTORY:
;  - 05/11/2013: inital verison 
;
;-
; LICENCE:
; Copyright (C) 2013 Marc Schellens
; This program is free software; you can redistribute it and/or modify  
; it under the terms of the GNU General Public License as published by  
; the Free Software Foundation; either version 2 of the License, or     
; (at your option) any later version.
;-
;

pro tidyManagedCommon
common managed_by_gdl, ids, names
if (not keyword_set(ids)) then begin ids=0 & names = 0 & endif
if (ids[0] eq 0) then return
keep=where(widget_info(ids,/managed),count)
if ( count gt 0 ) then begin
  ids=(temporary(ids))[keep]
  names=(temporary(names))[keep]
endif else begin
  ids = 0
  names = 0
endelse
end

pro XMANAGER, name, id, NO_BLOCK = noBlock, GROUP_LEADER=groupLeader, EVENT_HANDLER=eventHandler, CLEANUP=Cleanup

common managed_by_gdl, ids, names

tidyManagedCommon

if not keyword_set(eventHandler) then begin
  eventHandler = name + '_event'
endif
 
widget_control, id, event_pro=eventHandler
widget_control, id, /managed
; add to common
if (ids[0] ne 0) then begin
 ids = [ids, id]
 names = [names, name]
endif else begin
 ids = id
 names = name
endelse

if keyword_set(groupLeader) then begin
   widget_control, id, GROUP_LEADER=groupLeader
endif
; dummy function at the moment -- even KILL_NOTIFY is not implemented
if n_elements(cleanup) then begin
   widget_control, id, KILL_NOTIFY=Cleanup
endif

if keyword_set(noBlock) then begin

   widget_control, /XMANAGER_ACTIVE_COMMAND, id
endif else begin

   tmp = widget_event(/XMANAGER_BLOCK)
endelse

end

