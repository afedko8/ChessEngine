# CMakeList.txt : CMake project for Mchess2, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (Mchess2 chess.cpp "Constants.h" Prints.h Tests.h UCI.ipp MoveGeneration.ipp Attacks.ipp Macros_and_lib.ipp)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Mchess2 PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
