project(SomeProject)
cmake_minimum_required(VERSION 3.24)
find_package(Kokkos REQUIRED)
find_package(OpenMP REQUIRED)
find_package(MPerf REQUIRED)

add_executable(exec Exec.cpp)

add_executable(dobasic1000 DoBasic1000.cpp)
add_executable(dobasic10000 DoBasic10000.cpp)
add_executable(dobasic100000 DoBasic100000.cpp)
target_link_libraries(dobasic1000 MPerf::MPerf)
target_link_libraries(dobasic10000 MPerf::MPerf)
target_link_libraries(dobasic100000 MPerf::MPerf)

add_executable(dokokkos1000 DoKokkos1000.cpp)
add_executable(dokokkos10000 DoKokkos10000.cpp)
add_executable(dokokkos100000 DoKokkos100000.cpp)
target_link_libraries(dokokkos1000 MPerf::MPerf Kokkos::kokkos)
target_link_libraries(dokokkos10000 MPerf::MPerf Kokkos::kokkos)
target_link_libraries(dokokkos100000 MPerf::MPerf Kokkos::kokkos)

add_executable(doopenmp1000 DoOpenMP1000.cpp)
add_executable(doopenmp10000 DoOpenMP10000.cpp)
add_executable(doopenmp100000 DoOpenMP100000.cpp)
target_link_libraries(doopenmp1000 MPerf::MPerf OpenMP::OpenMP_CXX)
target_link_libraries(doopenmp10000 MPerf::MPerf OpenMP::OpenMP_CXX)
target_link_libraries(doopenmp100000 MPerf::MPerf OpenMP::OpenMP_CXX)

