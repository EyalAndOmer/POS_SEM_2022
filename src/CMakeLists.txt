link_directories(
        ${GTKMM_LIBRARY_DIRS}  )

include_directories(
        ${GTKMM_INCLUDE_DIRS}  )

add_executable(${PROJECT_NAME}
    main.cpp
        windows/Application/Application.cpp
        windows/ConnectedWindow/ConnectedWindow.cpp
        windows/ServerSetup/ServerSetupWindow.cpp
    ${GENERATED_DATA_DIR}/resources.c
        windows/FileDownloadWindow/FileDownloadWindow.cpp windows/FileDownloadWindow/FileDownloadWindow.h windows/MainAppWindow/MainAppWindow.cpp windows/MainAppWindow/MainAppWindow.h)

target_include_directories(${PROJECT_NAME} PRIVATE ${GENERATED_SOURCES_DIR})

compile_resources(
    ${GENERATED_DATA_DIR}/resources.c
    main_window.ui
    factory_resource.ui
    file_start_download.ui
    )

install(TARGETS ${PROJECT_NAME} DESTINATION bin)

target_link_libraries(${PROJECT_NAME}
        ${GTKMM_LIBRARIES}  )
