set(SOURCES
CFMTLexer.cpp
CFMTLexer.hpp
CFMTTokenTypes.hpp
FMTIn.cpp
FMTIn.hpp
FMTInTokenTypes.hpp
FMTLexer.cpp
FMTLexer.hpp
FMTOut.cpp
FMTOut.hpp
FMTOutTokenTypes.hpp
FMTParser.cpp
FMTParser.hpp
FMTTokenTypes.hpp
GDLInterpreter.cpp
GDLInterpreter.hpp
GDLInterpreterTokenTypes.hpp
GDLLexer.cpp
GDLLexer.hpp
GDLParser.cpp
GDLParser.hpp
GDLTokenTypes.hpp
GDLTreeParser.cpp
GDLTreeParser.hpp
GDLTreeParserTokenTypes.hpp
accessdesc.hpp
allix.hpp
allix.cpp
arrayindex.cpp
arrayindex.hpp
arrayindexlistt.hpp
arrayindexlistnoassoct.hpp
assocdata.cpp
assocdata.hpp
basegdl.cpp
basegdl.hpp
basic_fun.cpp
basic_fun.hpp
basic_fun_cl.cpp
basic_fun_cl.hpp
basic_fun_jmg.cpp
basic_fun_jmg.hpp
basic_op.cpp
cformat.g
color.cpp
color.hpp
convert2.cpp
convol.cpp
convol_inc0.cpp
convol_inc1.cpp
convol_inc2.cpp
datalistt.hpp
datatypes.cpp
datatypes.hpp
datatypesref.cpp
dcommon.cpp
dcommon.hpp
dcompiler.cpp
dcompiler.hpp
default_io.cpp
deviceps.hpp
devicesvg.hpp
devicez.hpp
dimension.hpp
dialog.cpp
dialog.hpp
dinterpreter.cpp
dinterpreter.hpp
dnode.cpp
dnode.hpp
dnodefactory.hpp
dpro.cpp
dpro.hpp
dstructdesc.cpp
dstructdesc.hpp
dstructgdl.cpp
dstructgdl.hpp
dvar.cpp
dvar.hpp
envt.cpp
envt.hpp
extrat.cpp
extrat.hpp
fftw.cpp
fftw.hpp
file.cpp
file.hpp
fmtnode.cpp
fmtnode.hpp
format.g
format.in.g
format.out.g
gdlc.g
gdlc.i.g
gdlc.tree.g
gdleventhandler.cpp
gdleventhandler.hpp
gdlexception.cpp
gdlexception.hpp
gdlgstream.cpp
gdlgstream.hpp
)
if(WIN32)
set(SOURCES
${SOURCES}
gdlwinstream.cpp
gdlwinstream.hpp
gtdhelper.cpp
gtdhelper.hpp
devicewin.hpp
devicewin.cpp
)
else(WIN32)
set(SOURCES
${SOURCES}
devicex.hpp
gdlxstream.cpp
gdlxstream.hpp
)
endif(WIN32)
set(SOURCES
${SOURCES}
gdljournal.cpp
gdljournal.hpp
gdlpsstream.cpp
gdlpsstream.hpp
gdlpython.cpp
gdlpython.hpp
gdlsvgstream.cpp
gdlsvgstream.hpp
gdlwidget.cpp
gdlwidgeteventhandler.cpp
gdlwidget.hpp
gdlwxstream.cpp
gdlwxstream.hpp
gdlzstream.cpp
gdlzstream.hpp
getfmtast.cpp
getfmtast.hpp
graphicsdevice.cpp
graphicsdevice.hpp
basic_pro.cpp
basic_pro.hpp
basic_pro_jmg.cpp
basic_pro_jmg.hpp
grib.cpp
grib.hpp
gshhs.cpp
gshhs.hpp
gsl_fun.cpp
gsl_fun.hpp
gsl_matrix.cpp
gsl_matrix.hpp
gzstream.hpp
hash.cpp
hash.hpp
hdf5_fun.cpp
hdf5_fun.hpp
hdf_fun.cpp
hdf_fun.hpp
hdf_pro.cpp
hdf_pro.hpp
ifmt.cpp
image.cpp
image.hpp
initct.cpp
initsysvar.cpp
initsysvar.hpp
io.cpp
io.hpp
lapack.cpp
least_squares.cpp
least_squares.hpp
libinit.cpp
libinit_ac.cpp
libinit_cl.cpp
libinit_gm.cpp
libinit_jmg.cpp
libinit_mes.cpp
libinit_ng.cpp
libinit_jp.cpp
list.cpp
list.hpp
magick_cl.cpp
magick_cl.hpp
math_fun.cpp
math_fun.hpp
math_fun_ac.cpp
math_fun_ac.hpp
math_fun_gm.cpp
math_fun_gm.hpp
math_fun_jmg.cpp
math_fun_jmg.hpp
math_fun_ng.cpp
math_fun_ng.hpp
math_utl.cpp
math_utl.hpp
matrix_cholesky.cpp
matrix_cholesky.hpp
matrix_invert.cpp
matrix_invert.hpp
mpi.cpp
mpi.hpp
ncdf_att_cl.cpp
ncdf_cl.cpp
ncdf_cl.hpp
ncdf_dim_cl.cpp
ncdf_var_cl.cpp
ncdf4_group.cpp
new.cpp
new.hpp
newprognode.cpp
nullgdl.cpp
nullgdl.hpp
objects.cpp
objects.hpp
ofmt.cpp
ofmt.hpp
overload.cpp
overload.hpp
plotting_axis.cpp
plotting_contour.cpp
plotting_convert_coord.cpp
plotting_cursor.cpp
plotting_device.cpp
plotting_erase.cpp
plotting_image.cpp
plotting_map_proj.cpp
plotting_misc.cpp
plotting_oplot.cpp
plotting_plot.cpp
plotting_plots.cpp
plotting_polyfill.cpp
plotting_shade_surf.cpp
plotting_surface.cpp
plotting_windows.cpp
plotting_xyouts.cpp
plotting.cpp
plotting.hpp
print.cpp
print_tree.cpp
print_tree.hpp
prognode.cpp
prognode_lexpr.cpp
prognodeexpr.cpp
read.cpp
real2int.hpp
semshm.cpp
semshm.hpp
sigfpehandler.cpp
sigfpehandler.hpp
#smooth.cpp
#smooth.hpp
specializations.hpp
str.cpp
str.hpp
strassenmatrix.hpp
terminfo.cpp
terminfo.hpp
topython.cpp
typedefs.hpp
typetraits.cpp
typetraits.hpp
widget.cpp
widget.hpp
)

include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR})

if(SYSTEM_ANTLR)
	SET(LIBRARIES ${LIBRARIES} antlr-pic)
else(SYSTEM_ANTLR)
	add_subdirectory(antlr)
	include_directories(${CMAKE_SOURCE_DIR}/src)
	SET(LIBRARIES ${LIBRARIES} gdl_antlr)
endif(SYSTEM_ANTLR)

link_directories(${LINK_DIRECTORIES})

add_definitions(-DHAVE_CONFIG_H)

if (MINGW)
	SET(LIBRARIES ${LIBRARIES} ws2_32)
endif (MINGW)
      
if(SHARED_LIB)
	add_library(gnudatalanguage SHARED ${SOURCES})
	if(NOT SYSTEM_ANTLR)
		add_dependencies(gnudatalanguage antlr)
	endif(NOT SYSTEM_ANTLR)
	SET_TARGET_PROPERTIES(gnudatalanguage PROPERTIES SOVERSION 0)
	target_link_libraries(gnudatalanguage PRIVATE ${LIBRARIES})
	install(TARGETS gnudatalanguage DESTINATION lib)
endif(SHARED_LIB)

if (GDL_EXECUTABLE)
	if(SHARED_LIB)
		add_executable(gdl gdl.cpp)
		target_link_libraries(gdl gnudatalanguage)
		if (OPENMP)
			target_link_libraries(gdl -fopenmp)
		endif (OPENMP)
		if (READLINE)
			target_link_libraries(gdl readline)
		endif (READLINE)
	else(SHARED_LIB)
		add_executable(gdl ${SOURCES} gdl.cpp)
		if(NOT SYSTEM_ANTLR)
			add_dependencies(gdl antlr)
		endif(NOT SYSTEM_ANTLR)
		target_link_libraries(gdl ${LIBRARIES})
	endif(SHARED_LIB)
	install(TARGETS gdl DESTINATION bin)
endif (GDL_EXECUTABLE)

if(PYTHON_MODULE) #GDL.so
	if(SHARED_LIB)
		add_library(pythongdl SHARED pythongdl.cpp)
		target_link_libraries(pythongdl gnudatalanguage)
	else(SHARED_LIB)
		add_library(pythongdl SHARED ${SOURCES} pythongdl.cpp)
	endif(SHARED_LIB)
	if(NOT SYSTEM_ANTLR)
		add_dependencies(pythongdl antlr)
	endif(NOT SYSTEM_ANTLR)
	SET_TARGET_PROPERTIES(pythongdl PROPERTIES PREFIX "")
	SET_TARGET_PROPERTIES(pythongdl PROPERTIES OUTPUT_NAME GDL)
	SET_TARGET_PROPERTIES(pythongdl PROPERTIES NO_SONAME TRUE)
	install(TARGETS pythongdl DESTINATION ${PYTHON_SITE_DIR})
endif(PYTHON_MODULE)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/pro/ DESTINATION ${CMAKE_INSTALL_PREFIX}/${GDL_DATA_DIR}/lib
	PATTERN CVS EXCLUDE
	PATTERN checks EXCLUDE
	PATTERN Makefile* EXCLUDE
	PATTERN UrgentNeed.txt EXCLUDE)

