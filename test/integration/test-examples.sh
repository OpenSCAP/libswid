set -e

EXAMPLE_SWID="$1"
shift

for ex_exec in "$@"; do
        "$ex_exec" "$EXAMPLE_SWID" | grep -q '^Creator of ACME RoadRunner Management Suite software: The ACME corporation \[acme\.com\]$'
done

