find_library(READLINE_LIBRARY NAMES readline)
find_library(HISTORY_LIBRARY NAMES history)
set(READLINE_LIBRARIES ${READLINE_LIBRARY} ${HISTORY_LIBRARY})
find_path(READLINE_INCLUDE_DIR NAMES readline/readline.h)
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Readline DEFAULT_MSG READLINE_LIBRARIES READLINE_INCLUDE_DIR)

mark_as_advanced(
READLINE_LIBRARY
HISTORY_LIBRARY
READLINE_LIBRARIES
READLINE_INCLUDE_DIR
)
