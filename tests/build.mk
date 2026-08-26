TESTS_DIR     := tests
TESTS_OUT_DIR := $(OUT_DIR)/tests

ELASH_TESTS_DIR := $(TESTS_DIR)/elash
ELC_TESTS_DIR   := $(TESTS_DIR)/elc

ELASH_TESTS_SRCS := $(call rwildcard,$(ELASH_TESTS_DIR),*.c)
ELASH_TESTS_BINS := $(patsubst $(TESTS_DIR)/%.c,$(TESTS_OUT_DIR)/%$(EXE_EXT),$(ELASH_TESTS_SRCS))

ELC_TESTS_SRCS := $(call rwildcard,$(ELC_TESTS_DIR),*.c)
ELC_TESTS_BINS := $(patsubst $(TESTS_DIR)/%.c,$(TESTS_OUT_DIR)/%$(EXE_EXT),$(ELC_TESTS_SRCS))

E2E_TEST_RUNNER := $(TESTS_DIR)/e2e/runner.py

FUZZ_SRC    := $(TESTS_DIR)/fuzz/fuzzer.c
FUZZ_BINARY := $(TESTS_OUT_DIR)/fuzz/fuzzer$(EXE_EXT)
FUZZ_RUNNER := -m tests.fuzz.fuzz

UNPARSER_RUNNER_SRC := $(TESTS_DIR)/unparser/unparse.c
UNPARSER_RUNNER_BIN := $(TESTS_OUT_DIR)/unparser/unparse$(EXE_EXT)
UNPARSER_RUNNER 	:= -m tests.unparser.run

ifdef TEST_PARALLELISM
	TEST_PARALLEL_FLAG := -j$(TEST_PARALLELISM)
endif

.PHONY: test-dirs unit-test clean-tests clean-test
.PHONY: test test-verbose test-all test-all-verbose

test-dirs:
	@$(call CMD_MKDIR_P,$(TESTS_OUT_DIR))
unit-test: test-elash test-elc
	@echo "All tests passed."

test: test-elash test-elc test-e2e test-unparser
	@echo "All tests passed."
test-verbose: test-elash test-elc test-e2e-verbose test-unparser-verbose
	@echo "All tests passed."

test-all: test-elash test-elc test-e2e test-unparser test-fuzz
	@echo "All tests passed."
test-all-verbose: test-elash test-elc test-e2e-verbose test-unparser-verbose test-fuzz
	@echo "All tests passed."

clean-test: clean-tests
clean-tests:
	@$(call CMD_RM_RF,$(TESTS_OUT_DIR))

################ unit tests ##################
$(TESTS_OUT_DIR)/elash/%$(EXE_EXT): $(TESTS_DIR)/elash/%.c $(LIBELASH_STATIC) | test-dirs
	@$(call CMD_MKDIR_P,$(dir $@))
	$(ECHO) "LD $@"
	$(Q)$(CC) $(TESTS_CFLAGS) $< $(LIBELASH_STATIC) $(TESTS_LDFLAGS)  $(CRITERION) -o $@

$(TESTS_OUT_DIR)/elc/%$(EXE_EXT): $(TESTS_DIR)/elc/%.c $(LIBELC_STATIC) $(LIBELASH_STATIC) | test-dirs
	@$(call CMD_MKDIR_P,$(dir $@))
	$(ECHO) "CC $@"
	$(Q)$(CC) $(TESTS_CFLAGS) $< $(LIBELC_STATIC) $(LIBELASH_STATIC) $(TESTS_LDFLAGS) $(CRITERION) $(LLVM_LDFLAGS) -o $@

.PHONY: test-elash test-elc
test-elash: $(ELASH_TESTS_BINS)
	@$(foreach t,$(ELASH_TESTS_BINS), \
		echo "Running $(patsubst $(TESTS_OUT_DIR)/%,%,$(t))..." && \
		$(call FIXPATH,$(t)) && \
	) true

test-elc: $(ELC_TESTS_BINS)
	@$(foreach t,$(ELC_TESTS_BINS), \
		echo "Running $(patsubst $(TESTS_OUT_DIR)/%,%,$(t))..." && \
		$(call FIXPATH,$(t)) && \
	) true

############## e2e tests ###############
.PHONY: test-e2e test-e2e-verbose
test-e2e: $(ELC_BIN)
	@$(call CMD_MKDIR_P,$(TESTS_OUT_DIR)/e2e)
	@echo "Running End-To-End tests"
	@$(PY) $(E2E_TEST_RUNNER) $(ELC_BIN) $(TESTS_OUT_DIR)/e2e $(TEST_PARALLEL_FLAG)

test-e2e-verbose: $(ELC_BIN)
	@$(call CMD_MKDIR_P,$(TESTS_OUT_DIR)/e2e)
	@echo "Running End-To-End tests"
	@$(PY) $(E2E_TEST_RUNNER) $(ELC_BIN) $(TESTS_OUT_DIR)/e2e $(TEST_PARALLEL_FLAG) --verbose

############# fuzzing #################
$(FUZZ_BINARY): $(FUZZ_SRC) $(LIBELASH_STATIC) $(LIBELC_STATIC) | test-dirs
	@$(call CMD_MKDIR_P,$(dir $@))
	@$(ECHO) "CC $@"
	$(Q)$(CC) $(TESTS_CFLAGS) $< $(LIBELASH_STATIC) $(LIBELC_STATIC) $(LDFLAGS) -o $@

.PHONY: test-fuzz-% test-fuzz
test-fuzz-%: $(FUZZ_BINARY) $(ELC_BIN)
	@echo "Running fuzz with count $*"
	@$(PY) $(FUZZ_RUNNER) $(FUZZ_BINARY) $(ELC_BIN) $*

test-fuzz: $(FUZZ_BINARY) $(ELC_BIN)
	@echo "Running fuzzer"
	@$(PY) $(FUZZ_RUNNER) $(FUZZ_BINARY) $(ELC_BIN) $(or $(FUZZ_COUNT),200)

############ unparser tests ############
$(UNPARSER_RUNNER_BIN): $(UNPARSER_RUNNER_SRC) $(LIBELASH_STATIC) | test-dirs
	@$(call CMD_MKDIR_P,$(dir $@))
	@$(ECHO) "CC $@"
	$(Q)$(CC) $(TESTS_CFLAGS) $< $(LIBELASH_STATIC) $(TESTS_LDFLAGS) -o $@

.PHONY: test-unparser test-unparser-verbose
test-unparser: $(UNPARSER_RUNNER_BIN)
	@echo "Running unparser integration tests"
	@$(PY) $(UNPARSER_RUNNER) $(UNPARSER_RUNNER_BIN) $(TEST_PARALLEL_FLAG)

test-unparser-verbose: $(UNPARSER_RUNNER_BIN)
	@echo "Running unparser integration tests"
	@$(PY) $(UNPARSER_RUNNER) $(UNPARSER_RUNNER_BIN) $(TEST_PARALLEL_FLAG) --verbose
