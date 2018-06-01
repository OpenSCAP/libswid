#!/usr/bin/env python3

import sys

import swid


if __name__ == "__main__":
    if len(sys.argv) != 2:
        msg = (
            "No input file has been specified.\n"
            "Specify exactly one SWID tag filename."
        )
        print(msg, file=sys.stderr)
        sys.exit(1)

    tag_data = swid.SWIDStruct()
    xml_backend = "xerces"

    io = swid.SWIDTagIO()
    io.setBackend(xml_backend)
    if not io:
        msg = ("Error getting XML backend '{}', try another one."
               .format(xml_backend))
        print(msg, file=sys.stderr)
        sys.exit(1)
    try:
        tag_data = io.load(sys.argv[1])
    except swid.XMLIOError as exc:
        print(str(exc), file=sys.stderr)
        sys.exit(1)

    creator_not_found = True
    for ent in tag_data.entities:
        if ent.role & swid.SWID_ROLE_SOFTWARE_CREATOR:
            creator_not_found = False
            msg = ("Creator of {} software: {} [{}]"
                   .format(tag_data.name, ent.name, ent.regid))
            print(msg)
            break
    if creator_not_found:
        print("Creator of {} software has not been found.".format(tag_data.name))
