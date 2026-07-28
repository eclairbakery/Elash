.PHONY: setup-githooks
setup-githooks:
	git config --local core.hooksPath scripts/githooks
ifeq ($(PLATFORM),posix)
	chmod +x scripts/githooks/pre-commit
endif
