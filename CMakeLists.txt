cmake_minimum_required(VERSION 3.20)
project(DoublePendulum)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Détection automatique du mode Emscripten (Web) ou Natif
if(EMSCRIPTEN)
    message(STATUS "Mode de compilation : WebAssembly (Emscripten)")
    
    # Cible WebAssembly
    add_executable(pendulum_web src/main.cpp) # Mis à jour avec ton dossier src/
    
    # Options de COMPILATION indispensables pour qu'il trouve <SDL3/SDL.h>
    target_compile_options(pendulum_web PRIVATE "-O3" "-sUSE_SDL=3")
    
    # Options de LINKAGE spécifiques à Emscripten
    target_link_options(pendulum_web PRIVATE 
        "-O3"
        "-sUSE_SDL=3"
        "-sWASM=1"
        "--shell-file=${CMAKE_CURRENT_SOURCE_DIR}/template/template.html"
    )
    
    # Renommer la sortie en index.html pour le serveur web
    set_target_properties(pendulum_web PROPERTIES OUTPUT_NAME "index")
    set_target_properties(pendulum_web PROPERTIES SUFFIX ".html")
    
else()
    message(STATUS "Mode de compilation : Application Native")
    
    # Cible native PC
    add_executable(pendulum_native src/main.cpp)
    
    # Recherche de SDL3 sur ton système local
    find_package(SDL3 REQUIRED)
    
    target_link_libraries(pendulum_native PRIVATE SDL3::SDL3)
endif()
