
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

#executable test_lgscatgath
add_executable(testlgscatgath test_identicalLG_ScatterGather.cpp)
target_compile_features(testlgscatgath PRIVATE cxx_std_11)
target_compile_options(testlgscatgath PRIVATE -O3 )
target_link_libraries(testlgscatgath PRIVATE mrg MPI::MPI_CXX)
target_include_directories(testlgscatgath PRIVATE "${PROJECT_SOURCE_DIR}/include/Multi-RandGreedi")

#executable test_lgsndrcv
add_executable(testlgsndrcv test_identicalLG_SndRcv.cpp)
target_compile_features(testlgsndrcv PRIVATE cxx_std_11)
target_compile_options(testlgsndrcv PRIVATE -O3 )
target_link_libraries(testlgsndrcv PRIVATE mrg MPI::MPI_CXX)
target_include_directories(testlgsndrcv PRIVATE "${PROJECT_SOURCE_DIR}/include/Multi-RandGreedi")

#executable test_greeDiL
add_executable(testgreeDiL test_greeDiL.cpp)
target_compile_features(testgreeDiL PRIVATE cxx_std_11)
target_compile_options(testgreeDiL PRIVATE -O3 )
target_link_libraries(testgreeDiL PRIVATE mrg MPI::MPI_CXX)
target_include_directories(testgreeDiL PRIVATE "${PROJECT_SOURCE_DIR}/include/Multi-RandGreedi")

#executable test_greeDiL2
add_executable(testgreeDiL2 test_greeDiL2.cpp)
target_compile_features(testgreeDiL2 PRIVATE cxx_std_11)
target_compile_options(testgreeDiL2 PRIVATE -O3 )
target_link_libraries(testgreeDiL2 PRIVATE mrg MPI::MPI_CXX)
target_include_directories(testgreeDiL2 PRIVATE "${PROJECT_SOURCE_DIR}/include/Multi-RandGreedi")

#executable rdmpart
add_executable(rdmpart rdm_partition_mtx.cpp)
target_compile_features(rdmpart PRIVATE cxx_std_11)
target_compile_options(rdmpart PRIVATE -O3 )
target_link_libraries(rdmpart PRIVATE mrg)
target_include_directories(rdmpart PRIVATE "${PROJECT_SOURCE_DIR}/include/Multi-RandGreedi") 

