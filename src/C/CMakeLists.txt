project(osu_iforest)
add_definitions(--std=c99 -D_GNU_SOURCE -Wall)
set(HEADER_FILES argparse.h argparse_iforest.h common.h frames.h object.h readwrite.h strfun.h)
set(SOURCE_FILES argparse.c argparse_iforest.c common.c frames.c object.c readwrite.c strfun.c)
add_library(Clib STATIC ${SOURCE_FILES} ${HEADER_FILES})
