macro(compile_resources OUTPUT)
    find_program(GLIB_RESOURCE_COMPILER NAMES glib-compile-resources REQUIRED)

    set(GRESOURCE_FILE ${GENERATED_DATA_DIR}/${PROJECT_NAME}.gresource.xml)
    set(WORK_DIR ${PROJECT_SOURCE_DIR}/data)

    if(${ARGC} GREATER 1)
        foreach(arg IN ITEMS ${ARGN})
            string(CONCAT RESOURCE ${WORK_DIR}/ui/ ${arg})
            list(APPEND RESOURCES ${RESOURCE})
        endforeach()
    endif()

    add_custom_command(
        OUTPUT ${OUTPUT}
        WORKING_DIRECTORY ${WORK_DIR}
        COMMAND ${GLIB_RESOURCE_COMPILER} --target=${OUTPUT} --generate-source ${GRESOURCE_FILE}
        DEPENDS ${GRESOURCE_FILE} ${RESOURCES}
        COMMENT "Generating ${OUTPUT}..."
        )
endmacro()
