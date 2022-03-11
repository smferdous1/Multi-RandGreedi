
set(HEADER_LIST
    "${PROJECT_SOURCE_DIR}/include/Multi-RandGreedi/CSR.h"
    "${PROJECT_SOURCE_DIR}/include/Multi-RandGreedi/Optimizer.h"
    "${PROJECT_SOURCE_DIR}/include/Multi-RandGreedi/Selection.h"
    "${PROJECT_SOURCE_DIR}/include/Multi-RandGreedi/Types.h"
    "${PROJECT_SOURCE_DIR}/include/Multi-RandGreedi/Utility.h"
    "${PROJECT_SOURCE_DIR}/include/Multi-RandGreedi/MPI_Types.h"
    )

add_library(mrg
    CSR.cpp
    LazyGreedy.cpp
    GreeDiL.cpp

    MaxSetCoverSelection.cpp
    KMedoidSelection.cpp
    BMatchingSelection.cpp 
    
    ${HEADER_LIST}
    )


target_include_directories(mrg PUBLIC
    "${PROJECT_SOURCE_DIR}/include/Multi-RandGreedi"
    )

# All users of this library will need at least C++11
target_compile_features(mrg PUBLIC cxx_std_11)
#target_compile_options(ra PRIVATE -O2 -fopenmp)
target_compile_options(mrg PRIVATE -O3)
