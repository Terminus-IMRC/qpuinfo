PROG:=qpuinfo
SRCS:=main.c mapmem.c pagesize.c mailbox/mailbox.c
ALLDEPS:=
CFLAGS:=-Wall -Wextra
CC:=gcc
RM:=rm -f
SUDO:=sudo
TOCLEAN:=

TARGETS:=all $(PROG) %.c.o %.c.d run clean
TARGETS_NO_NEED_DEPS:=clean
OBJS:=$(SRCS:%.c=%.c.o)
DEPS:=$(SRCS:%.c=%.c.d)

COMPILE.c=$(CC) $(CFLAGS) $(TARGET_ARCH) -c
COMPILE.d=$(CC) $(CFLAGS) $(TARGET_ARCH) -M -MP -MT $<.o -MF $@
LINK.o=$(CC) $(LDFLAGS) $(TARGET_ARCH)

all: $(PROG)

EXTRA_TARGETS:=$(filter-out $(TARGETS), $(MAKECMDGOALS))
ifneq '$(EXTRA_TARGETS)' ''
 $(error No rule to make target `$(word 1, $(EXTRA_TARGETS))')
else
 ifeq '$(filter-out $(TARGETS_NO_NEED_DEPS), $(MAKECMDGOALS))' '$(MAKECMDGOALS)'
  sinclude $(DEPS)
 else
  ifneq '$(words $(MAKECMDGOALS))' '1'
   $(error Specify only one target if you want to make a target that needs no dependency file)
  endif
 endif
endif

MAKEFILE_LIST_SANS_DEPS:=$(filter-out %.c.d, $(MAKEFILE_LIST))
ALLDEPS+=$(MAKEFILE_LIST_SANS_DEPS)

$(PROG): $(OBJS) $(ALLDEPS)
	$(LINK.o) $(OUTPUT_OPTION) $(OBJS) $(LOADLIBES) $(LDLIBS)

%.c.o: %.c $(ALLDEPS)
	$(COMPILE.c) $(OUTPUT_OPTION) $<

%.c.d: %.c $(ALLDEPS)
	$(COMPILE.d) $(OUTPUT_OPTION) $<

.PHONY: run
run: $(PROG)
	sudo ./$<

.PHONY: clean
clean:
	$(RM) $(PROG)
	$(RM) $(OBJS)
	$(RM) $(DEPS)
	$(RM) $(TOCLEAN)
