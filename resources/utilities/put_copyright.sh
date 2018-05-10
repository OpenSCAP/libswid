#!/bin/bash
# Add copyright stuff to those source files.
# Run without arguments.

COPYRIGHT="/*
 * libswid, The SWID Tag Manipulation Library
 * Copyright (C) 2018, Red Hat Security Compliance Team.
 *
 * Red Hat, Inc., hereby disclaims all copyright interes
 * in the library 'libswid'.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */"

SRCDIR=../../src
FILES="$(find $SRCDIR -name '*.h' -o -name '*.cpp' -o -name libswid)"

find_copyright_end() {
	local fname="$1"
	grep -n '^ \*/' "$fname" | head -n 1 | cut -f 1 -d :
}


for file in $FILES; do
	copyright_ends=$(find_copyright_end "$file")
	test -n "$copyright_ends" || copyright_ends=0
	file_past_copyright="$(tail -n +$((copyright_ends + 1)) "$file")"
	printf "%s\n" "$COPYRIGHT" "$file_past_copyright" > "$file"
done
