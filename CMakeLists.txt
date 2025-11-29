cmake_minimum_version_required(VERSION 3.10)
project(MiniFileExplorer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(MiniFileExplorer
    src/main.cpp
    src/App.cpp
    src/Commands.cpp
    src/Command.cpp
    src/CommandParser.cpp
    src/FsUtil.cpp
)

# On some platforms you may need to link stdc++fs for older compilers:
# target_link_libraries(MiniFileExplorer stdc++fs)