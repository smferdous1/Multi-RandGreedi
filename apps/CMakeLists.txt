
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
