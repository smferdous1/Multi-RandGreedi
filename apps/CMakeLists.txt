
#executable mpi-hello
add_executable(mpi_hello mpi-hello.cpp)
#target_compile_features(grdmatch PRIVATE cxx_std_11)
#target_compile_options(grdmatch PRIVATE -O3 )
target_link_libraries(mpi_hello PRIVATE MPI::MPI_CXX)
#target_include_directories(grdmatch PRIVATE "${PROJECT_SOURCE_DIR}/include/EC")

