#!/usr/bin/env bash

test-all() {
    local commit="${1:-HEAD}"
    local discovery_script="./testDiscovery.sh"
    local execution_script="./testExecution.sh"

    local discovery
    if ! discovery="$("$discovery_script")"; then
        echo "testDiscovery failed" >&2
        return 1
    fi

    local input_file
    input_file="$(mktemp)"
    trap 'rm -f "$input_file"' RETURN

    {
        printf '%s\n' '<?xml version="1.0" encoding="utf-8"?>'
        printf '%s\n' '<testAuditorInput version="1.0">'

        printf '%s\n' '  <testDiscovery>'
        emit_cdata "$discovery"
        printf '\n%s\n' '  </testDiscovery>'

        printf '%s\n' '  <reports>'

        local note
        if note="$(git notes --ref="commits" show "$commit" 2>/dev/null)"; then
            if [[ -n "$note" ]]; then
                printf '%s\n' '    <report format="junit-xml">'
                emit_cdata "$note"
                printf '\n%s\n' '    </report>'
            fi
        fi

        printf '%s\n' '  </reports>'
        printf '%s\n' '</testAuditorInput>'
    } > "$input_file"

    echo "===== input to testAuditor =====" >&2
    cat "$input_file" >&2
    echo "===== end input =====" >&2

    local auditor_output
    if ! auditor_output="$(nix develop --command testAuditor < "$input_file")"; then
        echo "testAuditor failed" >&2
        return 1
    fi

    if [[ -z "$auditor_output" ]]; then
        echo "testAuditor produced no output; testExecution skipped" >&2
        return 0
    fi

    echo "testAuditor output: $auditor_output" >&2

    printf '%s' "$auditor_output" | "$execution_script"
}

emit_cdata() {
    local content="$1"

    # Falls im Inhalt selbst "]]>" vorkommt, muss CDATA gesplittet werden.
    content=${content//]]>/]]]]><![CDATA[>}

    printf '<![CDATA[\n%s\n]]>' "$content"
}
