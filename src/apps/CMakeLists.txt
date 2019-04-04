#list(REMOVE_ITEM APPS CMakeLists.txt)
set(APPS "mcp")
set(CMAKE_EXE_LINKER_FLAGS "-T ${USER_LINKER_SCRIPT} -ffreestanding -std=gnu11 -nostdlib -static-libgcc -static-libstdc++ -lgcc")

foreach(APP ${APPS})
	add_subdirectory(${APP})
	list(APPEND APPS_TARGET_FILE ${APP}/${APP})
endforeach(APP)

file(GLOB_RECURSE FILES ${PROJECT_SOURCE_DIR}/src/initrd/*)
message(${FILES})

add_custom_command(
	OUTPUT initrd.tar
	COMMAND tar --absolute-names -c --transform='s:^.*/::' --group=0 --owner=0 --mode=500 -f initrd.tar ${APPS_TARGET_FILE} ${FILES}
	COMMENT "Creating initrd.tar"
	DEPENDS ${APPS}
)

add_custom_command(
	OUTPUT ${INITRD_TAR}
	COMMAND ${CMAKE_COMMAND} -E copy initrd.tar ${ISODIR}/boot
	COMMENT "Copying initrd.tar"
	DEPENDS initrd.tar
)

add_custom_target(build_initrd
	DEPENDS ${INITRD_TAR}
)
