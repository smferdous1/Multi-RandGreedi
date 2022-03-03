
set(HEADER_LIST
    "${PROJECT_SOURCE_DIR}/include/Multi-RandGreedi/CSR.h"

    )

add_library(mrg
    CSR.cpp
    ${HEADER_LIST}
    )


target_include_directories(mrg PUBLIC
    "${PROJECT_SOURCE_DIR}/include/Multi-RandGreedi"
    )

# All users of this library will need at least C++11
target_compile_features(mrg PUBLIC cxx_std_11)
#target_compile_options(ra PRIVATE -O2 -fopenmp)
target_compile_options(mrg PRIVATE -O3)
