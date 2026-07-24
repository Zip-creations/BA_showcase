SHELL := bash
.SHELLFLAGS := -eu -o pipefail -c
.ONESHELL:

CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++23 -Iclasses

AUDITOR_BIN ?= testAuditor

OUT_DIR = tests/out
BUILD_DIR = build
TODO_SRC = classes/ToDoList.cpp

# gTest
GTEST_BIN = $(BUILD_DIR)/gtest_tests
GTEST_SRC = tests/gtest/test_simple.cpp tests/gtest/test_identity.cpp
GTEST_OUT = $(OUT_DIR)/gtest
GTEST_REPORT_DIR = $(GTEST_OUT)/reports

# Catch2
CATCH_BIN = $(BUILD_DIR)/catch_tests
CATCH_SRC = tests/catch2/test_simple.cpp tests/catch2/test_identity.cpp
CATCH_OUT = $(OUT_DIR)/catch2
CATCH_REPORT_DIR = $(CATCH_OUT)/reports

.PHONY: all clean check-tools
.PHONY: test_gtest discover_gtest pick_gtest execute_gtest audit_gtest
.PHONY: test_catch discover_catch pick_catch execute_catch audit_catch

all: test_gtest test_catch

check-tools:
	command -v "$(AUDITOR_BIN)" >/dev/null 2>&1 || {
		echo "$(AUDITOR_BIN) not found in PATH" >&2
		exit 1
	}

$(GTEST_BIN): $(GTEST_SRC) $(TODO_SRC)
	mkdir -p "$(BUILD_DIR)"
	$(CXX) $(CXXFLAGS) $(GTEST_SRC) $(TODO_SRC) -o "$(GTEST_BIN)" -lgtest -lgtest_main -pthread

$(CATCH_BIN): $(CATCH_SRC) $(TODO_SRC)
	mkdir -p "$(BUILD_DIR)"
	$(CXX) $(CXXFLAGS) $(CATCH_SRC) $(TODO_SRC) -o "$(CATCH_BIN)"

test_gtest: $(GTEST_BIN)
	mkdir -p "$(GTEST_REPORT_DIR)"
	report="$(GTEST_REPORT_DIR)/full-$$(date -u +%Y%m%dT%H%M%S).xml"
	"./$(GTEST_BIN)" --gtest_output=xml:"$$report"
	cat "$$report"
	printf '\n'

test_catch: $(CATCH_BIN)
	mkdir -p "$(CATCH_REPORT_DIR)"
	report="$(CATCH_REPORT_DIR)/full-$$(date -u +%Y%m%dT%H%M%S).xml"
	"./$(CATCH_BIN)" -r junit -o "$$report"
	cat "$$report"
	printf '\n'

discover_gtest: $(GTEST_BIN)
	echo '<?xml version="1.0" encoding="utf-8"?>'
	echo '<testsuite>'
	"./$(GTEST_BIN)" --gtest_list_tests | awk '\
		/Running main/ { next } \
		/\.$$/{ suite=$$0; sub(/\.$$/, "", suite); next } \
		/^[[:space:]]/ { \
			gsub(/^ +/, "", $$0); \
			name=$$0; \
			printf("    <testcase classname=\"%s\" name=\"%s\" qualifiedName=\"%s.%s\"/>\n", suite, name, suite, name); \
		}'
	echo '</testsuite>'

discover_catch: $(CATCH_BIN)
	@echo '<?xml version="1.0" encoding="utf-8"?>'
	@echo '<testsuite>'
	@{ "./$(CATCH_BIN)" --list-tests || true; } | awk '\
		/^All available/ { next } \
		/^[0-9]+ test case/ { next } \
		/^[[:space:]]*$$/ { next } \
		{ \
			gsub(/^ +/, "", $$0); \
			name=$$0; \
			printf("    <testcase classname=\"catch_tests.global\" name=\"%s\" qualifiedName=\"%s\"/>\n", name, name); \
		}'
	@echo '</testsuite>'

# --------------------------------------------------------------------
# execute explicitly selected tests
# Usage:
#   make pick_gtest TESTS="Suite.TestA Suite.TestB"
#   make pick_catch TESTS="test_a test_b"
#
# stdin variants:
#   printf '%s\n' "Suite.TestA" "Suite.TestB" | make execute_gtest
#   printf '%s\n' "test_a" "test_b" | make execute_catch
# --------------------------------------------------------------------

pick_gtest: $(GTEST_BIN)
	if [[ -z "$${TESTS:-}" ]]; then
		echo 'Usage: make pick_gtest TESTS="Suite.TestA Suite.TestB"' >&2
		exit 1
	fi
	printf '%s\n' $$TESTS | $(MAKE) execute_gtest

pick_catch: $(CATCH_BIN)
	if [[ -z "$${TESTS:-}" ]]; then
		echo 'Usage: make pick_catch TESTS="test_a test_b"' >&2
		exit 1
	fi
	printf '%s\n' $$TESTS | $(MAKE) execute_catch

execute_gtest: $(GTEST_BIN)
	mapfile -t tests
	echo "gTest execution received $${#tests[@]} tests" >&2

	if [[ "$${#tests[@]}" -eq 0 ]]; then
		exit 0
	fi

	mkdir -p "$(GTEST_REPORT_DIR)"
	report="$(GTEST_REPORT_DIR)/pick-$$(date -u +%Y%m%dT%H%M%S).xml"
	filter="$$(printf '%s\n' "$${tests[@]}" | paste -sd:)"

	"./$(GTEST_BIN)" \
		--gtest_filter="$$filter" \
		--gtest_output=xml:"$$report" >&2

	cat "$$report"
	printf '\n'

execute_catch: $(CATCH_BIN)
	mapfile -t tests
	echo "Catch2 execution received $${#tests[@]} tests" >&2

	if [[ "$${#tests[@]}" -eq 0 ]]; then
		exit 0
	fi

	mkdir -p "$(CATCH_REPORT_DIR)"
	report="$(CATCH_REPORT_DIR)/pick-$$(date -u +%Y%m%dT%H%M%S).xml"

	"./$(CATCH_BIN)" "$${tests[@]}" -r junit -o "$$report" >&2
	cat "$$report"
	printf '\n'

audit_gtest: $(GTEST_BIN) check-tools
	tmp_dir="$$(mktemp -d)"
	trap 'rm -rf "$$tmp_dir"' EXIT

	discovery="$$tmp_dir/discovery.xml"
	input="$$tmp_dir/testAuditorInput.xml"
	selected="$$tmp_dir/selected-tests.txt"

	$(MAKE) discover_gtest > "$$discovery"
	$(MAKE) auditor_input DISCOVERY="$$discovery" REPORT_DIR="$(GTEST_REPORT_DIR)" > "$$input"

	"$(AUDITOR_BIN)" < "$$input" > "$$selected"

	echo "testAuditor selected gTest tests:" >&2
	if [[ -s "$$selected" ]]; then
		cat "$$selected" >&2
	else
		echo "(none)" >&2
	fi

	cat "$$selected" | $(MAKE) execute_gtest

audit_catch: $(CATCH_BIN) check-tools
	tmp_dir="$$(mktemp -d)"
	trap 'rm -rf "$$tmp_dir"' EXIT

	discovery="$$tmp_dir/discovery.xml"
	input="$$tmp_dir/testAuditorInput.xml"
	selected="$$tmp_dir/selected-tests.txt"

	$(MAKE) discover_catch > "$$discovery"
	$(MAKE) auditor_input DISCOVERY="$$discovery" REPORT_DIR="$(CATCH_REPORT_DIR)" > "$$input"

	"$(AUDITOR_BIN)" < "$$input" > "$$selected"

	echo "testAuditor selected Catch2 tests:" >&2
	if [[ -s "$$selected" ]]; then
		cat "$$selected" >&2
	else
		echo "(none)" >&2
	fi

	cat "$$selected" | $(MAKE) execute_catch

# --------------------------------------------------------------------
# internal helper target
# Builds testAuditorInput XML from:
#   DISCOVERY=<path>
#   REPORT_DIR=<directory with JUnit XML reports>
# --------------------------------------------------------------------

auditor_input:
	if [[ -z "$${DISCOVERY:-}" || -z "$${REPORT_DIR:-}" ]]; then
		echo 'Usage: make auditor_input DISCOVERY=... REPORT_DIR=...' >&2
		exit 1
	fi

	echo '<?xml version="1.0" encoding="utf-8"?>'
	echo '<testAuditorInput version="1.0">'

	echo '  <testDiscovery>'
	echo '<![CDATA['
	sed 's/]]>/]]]]><![CDATA[>/g' "$$DISCOVERY"
	echo ']]>'
	echo '  </testDiscovery>'

	echo '  <reports>'
	if [[ -d "$$REPORT_DIR" ]]; then
		for report in "$$REPORT_DIR"/*.xml; do
			[[ -f "$$report" ]] || continue

			echo '    <report format="junit-xml">'
			echo '<![CDATA['
			sed 's/]]>/]]]]><![CDATA[>/g' "$$report"
			echo ']]>'
			echo '    </report>'
		done
	fi
	echo '  </reports>'
	echo '</testAuditorInput>'

clean:
	rm -rf "$(BUILD_DIR)" "$(OUT_DIR)"
