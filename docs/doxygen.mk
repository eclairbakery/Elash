DOXYGEN ?= doxygen

DOCS_DIR	 := docs
DOCS_OUT_DIR := $(DOCS_DIR)/out

.PHONY: doc redoc doc-elash doc-elc clean-doc

doc: doc-elash #doc-elc

doc-elash: $(DOCS_DIR)/Doxyfile.elash
	@$(call CMD_MKDIR_P,$(DOCS_OUT_DIR)/elash)
	$(DOXYGEN) $(DOCS_DIR)/Doxyfile.elash

doc-elc: #$(DOCS_DIR)/Doxyfile.elc
#	@$(call CMD_MKDIR_P,$(dir $@))
#	$(DOXYGEN) $(DOCS_DIR)/Doxyfile.elc
	@echo "no libelc documentation yet"

clean-doc:
	@$(call CMD_RM_RF,$(DOCS_OUT_DIR))

redoc:
	@$(MAKE) clean-doc
	@$(MAKE) doc
