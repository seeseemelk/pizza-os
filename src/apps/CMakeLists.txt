#list(REMOVE_ITEM APPS CMakeLists.txt)
set(APPS "test01")

foreach(APP ${APPS})
	add_subdirectory(${APP})
	list(APPEND APPS_TARGET_FILE ${APP}/${APP})
endforeach(APP)

#add_custom_command(
#	OUTPUT initrd.tar
#	COMMAND tar cf initrd.tar ${APPS}
#	DEPENDS ${APPS}
#	COMMENT "Building initrd.tar"
#)
message(${APPS_TARGET_FILE})
add_custom_command(
	OUTPUT initrd.tar
	COMMAND tar -c --transform='s:^.*/::' --group=0 --owner=0 --mode=500 -f initrd.tar ${APPS_TARGET_FILE}
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

#add_custom_command(TARGET build_initrd
	#OUTPUT ${INITRD_TAR}
	#DEPENDS initrd.tar
	#)

#
#add_custom_command(TARGET initrd.tar
#	POST_BUILD
#	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:initrd.tar> ${ISODIR/boot}
#)
