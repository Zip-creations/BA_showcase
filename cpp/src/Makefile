SHELL := bash
.SHELLFLAGS := -eu -o pipefail -c

CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++23

AUDITOR_BIN ?= testAuditor

# gTest
GTEST_BIN = build/gtest_tests
GTEST_SRC = tests/gtest/gtest.cpp
GTEST_OUT = tests/out/gtest
GTEST_REPORT_DIR = $(GTEST_OUT)/reports
GTEST_WORK_DIR = $(GTEST_OUT)/work
GTEST_DISCOVERY = $(GTEST_WORK_DIR)/discovery.xml
GTEST_AUDITOR_INPUT = $(GTEST_WORK_DIR)/testAuditorInput.xml
GTEST_SELECTED = $(GTEST_WORK_DIR)/selected-tests.txt

# Catch2
CATCH_BIN = build/catch_tests
CATCH_SRC = tests/catch2/catch2.cpp
CATCH_OUT = tests/out/catch2
CATCH_REPORT_DIR = $(CATCH_OUT)/reports
CATCH_WORK_DIR = $(CATCH_OUT)/work
CATCH_DISCOVERY = $(CATCH_WORK_DIR)/discovery.xml
CATCH_AUDITOR_INPUT = $(CATCH_WORK_DIR)/testAuditorInput.xml
CATCH_SELECTED = $(CATCH_WORK_DIR)/selected-tests.txt

.PHONY: all test_gtest test_catch discover_gtest discover_catch audit_gtest audit_catch clean check-tools

all: test_gtest test_catch

check-tools:
	@command -v $(AUDITOR_BIN) >/dev/null 2>&1 || { echo "$(AUDITOR_BIN) not found in PATH" >&2; exit 1; }

# gtest build
$(GTEST_BIN): $(GTEST_SRC)
	mkdir -p build
	$(CXX) $(CXXFLAGS) $(GTEST_SRC) -o $(GTEST_BIN) -lgtest -lgtest_main -pthread

test_gtest: $(GTEST_BIN)
	@mkdir -p $(GTEST_REPORT_DIR)
	@report="$(GTEST_REPORT_DIR)/full-$$(date -u +%Y%m%dT%H%M%S).xml"; \
	./$(GTEST_BIN) --gtest_output=xml:"$$report"

# catch2 build
$(CATCH_BIN): $(CATCH_SRC)
	mkdir -p build
	$(CXX) $(CXXFLAGS) $(CATCH_SRC) -o $(CATCH_BIN)

test_catch: $(CATCH_BIN)
	@mkdir -p $(CATCH_REPORT_DIR)
	@report="$(CATCH_REPORT_DIR)/full-$$(date -u +%Y%m%dT%H%M%S).xml"; \
	./$(CATCH_BIN) -r junit -o "$$report"

$(GTEST_DISCOVERY): $(GTEST_BIN)
	@mkdir -p $(GTEST_WORK_DIR)
	@{ \
		echo '<?xml version="1.0" encoding="utf-8"?>'; \
		echo '<testsuite>'; \
		./$(GTEST_BIN) --gtest_list_tests | awk '\
			/Running main/ { next } \
			/\.$$/{ suite=$$0; sub(/\.$$/, "", suite); next } \
			/^[[:space:]]/ { \
				gsub(/^ +/, "", $$0); \
				name=$$0; \
				printf("    <testcase classname=\"%s\" name=\"%s\" qualifiedName=\"%s.%s\"/>\n", suite, name, suite, name); \
			}'; \
		echo '</testsuite>'; \
	} > "$@"

discover_gtest: $(GTEST_DISCOVERY)
	@cat "$<"

$(CATCH_DISCOVERY): $(CATCH_BIN)
	@mkdir -p $(CATCH_WORK_DIR)
	@{ \
		echo '<?xml version="1.0" encoding="utf-8"?>'; \
		echo '<testsuite>'; \
		./$(CATCH_BIN) --list-tests | awk '\
			/^All available/ { next } \
			/^[0-9]+ test case/ { next } \
			/^[[:space:]]*$$/ { next } \
			{ \
				gsub(/^ +/, "", $$0); \
				name=$$0; \
				printf("    <testcase classname=\"catch_tests.global\" name=\"%s\" qualifiedName=\"%s\"/>\n", name, name); \
			}'; \
		echo '</testsuite>'; \
	} > "$@"

discover_catch: $(CATCH_DISCOVERY)
	@cat "$<"

$(GTEST_AUDITOR_INPUT): $(GTEST_DISCOVERY)
	@mkdir -p $(GTEST_WORK_DIR)
	@{ \
		echo '<?xml version="1.0" encoding="utf-8"?>'; \
		echo '<testAuditorInput version="1.0">'; \
		echo '  <testDiscovery>'; \
		echo '<![CDATA['; \
		sed 's/]]>/]]]]><![CDATA[>/g' "$<"; \
		echo ']]>'; \
		echo '  </testDiscovery>'; \
		echo '  <reports>'; \
		for report in $(GTEST_REPORT_DIR)/*.xml; do \
			[[ -f "$$report" ]] || continue; \
			echo '    <report format="junit-xml">'; \
			echo '<![CDATA['; \
			sed 's/]]>/]]]]><![CDATA[>/g' "$$report"; \
			echo ']]>'; \
			echo '    </report>'; \
		done; \
		echo '  </reports>'; \
		echo '</testAuditorInput>'; \
	} > "$@"

$(CATCH_AUDITOR_INPUT): $(CATCH_DISCOVERY)
	@mkdir -p $(CATCH_WORK_DIR)
	@{ \
		echo '<?xml version="1.0" encoding="utf-8"?>'; \
		echo '<testAuditorInput version="1.0">'; \
		echo '  <testDiscovery>'; \
		echo '<![CDATA['; \
		sed 's/]]>/]]]]><![CDATA[>/g' "$<"; \
		echo ']]>'; \
		echo '  </testDiscovery>'; \
		echo '  <reports>'; \
		for report in $(CATCH_REPORT_DIR)/*.xml; do \
			[[ -f "$$report" ]] || continue; \
			echo '    <report format="junit-xml">'; \
			echo '<![CDATA['; \
			sed 's/]]>/]]]]><![CDATA[>/g' "$$report"; \
			echo ']]>'; \
			echo '    </report>'; \
		done; \
		echo '  </reports>'; \
		echo '</testAuditorInput>'; \
	} > "$@"

$(GTEST_SELECTED): $(GTEST_AUDITOR_INPUT) check-tools
	@$(AUDITOR_BIN) < "$<" > "$@"

$(CATCH_SELECTED): $(CATCH_AUDITOR_INPUT) check-tools
	@$(AUDITOR_BIN) < "$<" > "$@"

audit_gtest: $(GTEST_SELECTED) $(GTEST_BIN)
	@if [[ ! -s "$(GTEST_SELECTED)" ]]; then \
		echo "testAuditor selected no gtest tests"; \
		exit 0; \
	fi; \
	mkdir -p $(GTEST_REPORT_DIR); \
	report="$(GTEST_REPORT_DIR)/audit-$$(date -u +%Y%m%dT%H%M%S).xml"; \
	filter="$$(paste -sd: "$(GTEST_SELECTED)")"; \
	echo "running gtest filter: $$filter"; \
	./$(GTEST_BIN) --gtest_filter="$$filter" --gtest_output=xml:"$$report"

audit_catch: $(CATCH_SELECTED) $(CATCH_BIN)
	@if [[ ! -s "$(CATCH_SELECTED)" ]]; then \
		echo "testAuditor selected no catch2 tests"; \
		exit 0; \
	fi; \
	mkdir -p $(CATCH_REPORT_DIR); \
	report="$(CATCH_REPORT_DIR)/audit-$$(date -u +%Y%m%dT%H%M%S).xml"; \
	mapfile -t tests < "$(CATCH_SELECTED)"; \
	echo "running catch2 tests:"; \
	printf '%s\n' "$${tests[@]}"; \
	./$(CATCH_BIN) "$${tests[@]}" -r junit -o "$$report"

clean:
	rm -rf build tests/out
