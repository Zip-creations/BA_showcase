#!/usr/bin/env bash
set -uo pipefail

RUN_ID="$(date -u +%Y%m%dT%H%M%S)"
REPORT_PATH="test/out/report-$RUN_ID.xml"

mapfile -t tests
echo "testExecution received ${#tests[@]} tests" >&2
status=0

if [[ "${#tests[@]}" -eq 0 ]]; then
    exit 0
else
    PYTHONPATH="code" python3 -m pytest "${tests[@]}" --junit-xml="$REPORT_PATH" >&2 || status=$?
fi

cat "$REPORT_PATH"
exit "$status"
