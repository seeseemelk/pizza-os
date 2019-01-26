#list(REMOVE_ITEM APPS CMakeLists.txt)
set(APPS "test01")

foreach(APP ${APPS})
	add_subdirectory(${APP})
endforeach(APP)

add_custom_command(
	OUTPUT initrd.tar
	COMMAND tar cf initrd.tar ${APPS}
	COMMAND ${CMAKE_COMMAND} -E copy initrd.tar ${ISODIR/boot}
	DEPENDS ${APPS}
	COMMENT "Building initrd.tar"
)

add_custom_target(build_initrd
	DEPENDS initrd.tar
)

#
#add_custom_command(TARGET initrd.tar
#	POST_BUILD
#	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:initrd.tar> ${ISODIR/boot}
#)
