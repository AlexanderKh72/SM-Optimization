cmake_minimum_required(VERSION 3.19)
project(OptimisationQt LANGUAGES CXX)



find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets PrintSupport)

qt_standard_project_setup()

qt_add_executable(OptimisationQt
    WIN32 MACOSX_BUNDLE
    main.cpp
    optimisation.cpp
    optimisation.h
    optimisation.ui
    dialog_setmethod.h dialog_setmethod.cpp dialog_setmethod.ui

    qcustomplot.cpp
    qcustomplot.h
    Optimizers/AbstrFunction.h Optimizers/AbstrOptimizer.h Optimizers/NelderMeadOptimizer.h Optimizers/Point.h Optimizers/RectArea.h Optimizers/SimpleStochasticOptimizer.h
    Optimizers/NelderMeadOptimizer.cpp Optimizers/Point.cpp Optimizers/SimpleStochasticOptimizer.cpp
)

target_include_directories(OptimisationQt PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

if (MINGW)
    target_compile_options(OptimisationQt PRIVATE -Wa,-mbig-obj)
endif()

target_link_libraries(OptimisationQt
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt6::PrintSupport
)

include(GNUInstallDirs)

install(TARGETS OptimisationQt
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET OptimisationQt
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
