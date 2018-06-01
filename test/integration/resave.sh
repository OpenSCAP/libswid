#!/bin/bash


RESAVER="${RESAVER-./resaver}"
VERBOSE="${VERBOSE-off}"
verbose_opt='-quiet'
test "$VERBOSE" = off || verbose_opt=''


VALIDATOR_JAR="$1"
shift


# $1: What
# $2: Where
resave() {
	local rc
	$RESAVER "$1" "$2"
	rc=$?
	return $rc
}


# $1: What
validate() {
	local rc path name abspath
	path="$(dirname "$VALIDATOR_JAR")"
	name="$(basename "$VALIDATOR_JAR")"
	abspath="$1"
	printf "%s" "$abspath" | grep -q '^/' || abspath="$PWD/$abspath"
	(cd "$path" && java -jar "$name" -valreportfile "/tmp/swid_validator_report" -valresultfile "/tmp/swid_validator_result" $verbose_opt "$abspath")
	rc=$?
	return $rc
}


_die() {
	printf "%s\n" "Failed to resave and validate:"
	printf -- "- %s\n" "$@"
	exit 1
}


temp_dest=/tmp/lala

failed=()

for to_resave in "$@"; do
	resave "$to_resave" "$temp_dest" && validate "$temp_dest" || failed+=("$to_resave")
done

test "${#failed[*]}" -eq 0 || _die "${failed[@]}"
