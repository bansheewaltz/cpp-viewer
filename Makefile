BIN_DIR     ?= build
BUILD_TYPE  ?= Debug
BUILD_DIR   ?= $(BIN_DIR)/$(BUILD_TYPE)
INSTALL_DIR ?= installation

all: build
.PHONY: all

configure:
	cmake -B $(BUILD_DIR) -Wno-dev -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)

debug:   BUILD_TYPE = debug
release: BUILD_TYPE = release
build release debug: configure	
	cmake --build $(BUILD_DIR) --parallel -- --output-sync
.PHONY: build release debug

# verbose build
buildv: configure
	VERBOSE=1 cmake --build $(BUILD_DIR)
.PHONY: buildv

LINTFLAGS := --style=google
lint:     LINTFLAGS += -n
lint_fix: LINTFLAGS += -i
lint lint_fix:
	clang-format ${LINTFLAGS} $(shell find src -type f \
                                             -name '*.h'   -or -name '*.c' \
                                             -name '*.hpp' -or -name '*.cpp')
.PHONY: lint lint_fix

install: build
	cmake --install $(BUILD_DIR) --prefix $(INSTALL_DIR)
.PHONY: install

uninstall: build
	cmake --build $(BUILD_DIR) --target uninstall
	rm -rf $(INSTALL_DIR)
.PHONY: uninstall

clean:
	$(RM) -r build $(INSTALL_DIR)
	$(RM) *.tar* *.sh
.PHONY: clean

dvi:
	texi2dvi readmi.texi --clean
.PHONY: dvi

docs: configure
	cmake --build $(BUILD_DIR) --target docs
.PHONY: dvi

dist: build
	cpack --config $(BUILD_DIR)/CPackSourceConfig.cmake
	rm -r _CPack_Packages
.PHONY: dist

bdist: build
	cpack --config $(BUILD_DIR)/CPackConfig.cmake
	rm -r _CPack_Packages
.PHONY: bdist

test: configure
	cmake --build $(BUILD_DIR) --target run_tests
.PHONY: test

gcov_report: configure
	cmake --build $(BUILD_DIR) --target coverage
.PHONY: gcov_report
