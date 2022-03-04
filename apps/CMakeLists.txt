
#executable mpi-hello
add_executable(mpi_hello mpi-hello.cpp)
target_compile_features(mpi_hello PRIVATE cxx_std_11)
target_compile_options(mpi_hello PRIVATE -O3 )
target_link_libraries(mpi_hello PRIVATE MPI::MPI_CXX)
#target_include_directories(grdmatch PRIVATE "${PROJECT_SOURCE_DIR}/include/EC")

#executable test_input
add_executable(testio test_io.cpp)
target_compile_features(testio PRIVATE cxx_std_11)
target_compile_options(testio PRIVATE -O3 )
target_link_libraries(testio PRIVATE mrg)
target_include_directories(testio PRIVATE "${PROJECT_SOURCE_DIR}/include/Multi-RandGreedi")

#executable test_lazygreedy
add_executable(testlg test_lazygreedy.cpp)
target_compile_features(testlg PRIVATE cxx_std_11)
target_compile_options(testlg PRIVATE -O3 )
target_link_libraries(testlg PRIVATE mrg)
target_include_directories(testlg PRIVATE "${PROJECT_SOURCE_DIR}/include/Multi-RandGreedi")

#executable test_subMtx
add_executable(testsubMtx test_submtx.cpp)
target_compile_features(testsubMtx PRIVATE cxx_std_11)
target_compile_options(testsubMtx PRIVATE -O3 )
target_link_libraries(testsubMtx PRIVATE mrg)
target_include_directories(testsubMtx PRIVATE "${PROJECT_SOURCE_DIR}/include/Multi-RandGreedi")

#executable test_lgidentical
add_executable(testlgidentical test_identicalLG.cpp)
target_compile_features(testlgidentical PRIVATE cxx_std_11)
target_compile_options(testlgidentical PRIVATE -O3 )
target_link_libraries(testlgidentical PRIVATE mrg MPI::MPI_CXX)
target_include_directories(testlgidentical PRIVATE "${PROJECT_SOURCE_DIR}/include/Multi-RandGreedi")
