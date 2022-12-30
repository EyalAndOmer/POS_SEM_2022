# Link the resource file
set(GRESOURCE_FILE ${GENERATED_DATA_DIR}/${PROJECT_NAME}.gresource.xml)

# Make the resource file visible
configure_file(gresource.xml.in ${GRESOURCE_FILE})

# Init resource file script
install(SCRIPT ${PROJECT_SOURCE_DIR}/cmake/InstallScript.cmake)
