#!/bin/bash

command -v astyle >/dev/null 2>&1 || { echo >&2 "You need astyle installed to run this script!"; exit 1; }

# first we add brackets in single line statements after if ...
astyle --indent=spaces=4 --break-blocks --max-code-length=140 --suffix=none --break-after-logical --pad-oper --align-pointer=name --style=google --indent-preproc-define --indent-preproc-cond -J $@
# ... only to remove them a moment later to reach desired behavior (single line statements without brackets in a new line after if)
astyle --indent=spaces=4 --break-blocks --max-code-length=140 --suffix=none --break-after-logical --pad-oper --align-pointer=name --style=google --indent-preproc-define --indent-preproc-cond -xj $@

