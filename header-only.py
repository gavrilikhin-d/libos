import enum
from os import walk

_, _, filenames = next(walk("include/os"), (None, None, []))

ignore = ["libos.hpp", "macros.h", "version.hpp"]

from shutil import copy

import re

# Get content of a header file without license and header guard
def header_content(path: str, license_end_line: int = 29) -> str:
    with open(path) as h:
        lines = h.readlines()
        remove_indexes = []
        for (i, line) in enumerate(lines):
            if line.startswith("#include \""):
                remove_indexes.append(i)
        for i in reversed(remove_indexes):
            lines.pop(i)
        return ''.join(lines[license_end_line + 1:])

for filename in filenames:
    if filename in ignore: continue

    ho_path = "include/os/header-only/" + filename
    copy("include/os/" + filename, ho_path)
    with open(ho_path, "r+") as f:
        lines = f.readlines()

        # Update description
        description = "{}. Header-only".format(lines[0].removeprefix("// ").rstrip())
        lines[0] = f"// {description}\n"

        # Update file info
        for (i, line) in enumerate(lines):
            if line.startswith("/** @file"):
                lines[i] = "/** @file {}\n".format(ho_path.removeprefix("include/"))
                lines[i + 1] = f" *  {description}\n"
                break

        # Substitute includes
        for (i, line) in enumerate(lines):
            if line.startswith("#include \""):
                dependency = re.match("#include \"(.*)\"", line).group(1)
                content = header_content("include/" + dependency)
                lines[i] = \
                f"// #include \"{dependency}\"\n" \
                 "// =========================\n" \
                f"{content}\n"                    \
                f"// End of   \"{dependency}\"\n" \
                 "// =========================\n" \
                 "\n"

        f.seek(0)
        f.truncate()
        f.writelines(lines)

print(filenames)