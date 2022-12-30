# include gtkmm package
link_directories(
        ${GTKMM_LIBRARY_DIRS}  )

include_directories(
        ${GTKMM_INCLUDE_DIRS}  )

# add all the cpp executables  and the resources.c file
add_executable(${PROJECT_NAME}
    main.cpp
        windows/Application/Application.cpp
        windows/ConnectedWindow/ConnectedWindow.cpp
        windows/ServerSetup/ServerSetupWindow.cpp
    ${GENERATED_DATA_DIR}/resources.c
        windows/FileDownloadWindow/FileDownloadWindow.cpp
        windows/MainAppWindow/MainAppWindow.cpp
        windows/CurrentDownloadsWindow/CurrentDownloadsWindow.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE ${GENERATED_SOURCES_DIR})

# add all .ui files
compile_resources(
    ${GENERATED_DATA_DIR}/resources.c
    main_window.ui
    file_download_modal.ui
    )

install(TARGETS ${PROJECT_NAME} DESTINATION bin)

target_link_libraries(${PROJECT_NAME}
        ${GTKMM_LIBRARIES}  )
