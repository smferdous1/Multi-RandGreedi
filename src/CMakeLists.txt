
set(HEADER_LIST
    "${PROJECT_SOURCE_DIR}/include/Multi-RandGreedi/mtxReader.h"

    )

add_library(mrg
    mtxReader.cpp
    ${HEADER_LIST}
    )


target_include_dirmrgtories(mrg PUBLIC
    "${PROJECT_SOURCE_DIR}/include/Multi-RandGreedi"
    )

# All users of this library will need at least C++11
target_compile_features(mrg PUBLIC cxx_std_11)
#target_compile_options(ra PRIVATE -O2 -fopenmp)
target_compile_options(mrg PRIVATE -O3)
