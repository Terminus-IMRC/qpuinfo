set(QPUINFO_LDFLAGS      ${VC4REGMAP_LDFLAGS}      ${MAILBOX_LDFLAGS})
set(QPUINFO_INCLUDE_DIRS ${VC4REGMAP_INCLUDE_DIRS} ${MAILBOX_INCLUDE_DIRS})
set(QPUINFO_CFLAGS_OTHER ${VC4REGMAP_CFLAGS_OTHER} ${MAILBOX_CFLAGS_OTHER})
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pipe -g -O2 -W -Wall -Wextra")

add_executable(qpuinfo qpuinfo.c)
target_link_libraries(qpuinfo PRIVATE ${QPUINFO_LDFLAGS})
target_include_directories(qpuinfo PRIVATE ${QPUINFO_INCLUDE_DIRS})
target_compile_options(qpuinfo PRIVATE ${QPUINFO_CFLAGS_OTHER})

install(TARGETS qpuinfo RUNTIME DESTINATION sbin)
