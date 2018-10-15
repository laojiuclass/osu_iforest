project(osu_iforest)
include_directories(C)
add_subdirectory(C)
set(HEADER_FILES tree.hpp forest.hpp isolation_forest.hpp utility.hpp cincl.hpp r_forest.hpp main.hpp r_matrix.hpp DoubleFrame.hpp)
set(SOURCE_FILES tree.cpp forest.cpp isolation_forest.cpp utility.cpp oo_bestimator.hpp
        r_forest.cpp contribution.hpp)
add_library(iforestlib STATIC ${SOURCE_FILES} ${HEADER_FILES})
