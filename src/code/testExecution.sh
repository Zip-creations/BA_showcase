#!/usr/bin/env bash
set -uo pipefail

REPORT_PATH="test/out/reportNew.xml"
mkdir -p "$(dirname "$REPORT_PATH")"

mapfile -t tests

echo "testExecution received ${#tests[@]} tests" >&2

status=0

if [[ "${#tests[@]}" -eq 0 ]]; then
    cat > "$REPORT_PATH" <<'XML'
<?xml version="1.0" encoding="utf-8"?>
<testsuite name="pytest" tests="0" failures="0" errors="0" skipped="0">
</testsuite>
XML
else
    PYTHONPATH="code" python3 -m pytest "${tests[@]}" \
        --junit-xml="$REPORT_PATH" \
        >&2 || status=$?
fi

cat "$REPORT_PATH"

exit "$status"
