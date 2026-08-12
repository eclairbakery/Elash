export CC
export BUILD
export CFLAGS
export FUZZ_SRC
export MAIN_C_SRC
export LLVM_CFLAGS
export LIBELASH_C_SRCS
export LIBELC_C_SRCS
export ELASH_TESTS_SRCS
export ELC_TESTS_SRCS

.PHONY: compile-commands clean-compile-commands

compile-commands: compile_commands.json

compile_commands.json:
	$(ECHO) "PY $@"
	$(Q)$(PY) scripts/gen-comp-cmds.py

clean-compile-commands:
	@$(call CMD_RM_F,compile_commands.json)
