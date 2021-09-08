import enum
from os import walk

_, _, filenames = next(walk("include/os"), (None, None, []))

ignore = ["libos.hpp", "macros.h", "version.hpp"]

from shutil import copy

import re

# Remove #include "..." lines
def lines_without_user_includes(path: str) -> list[str]:
    with open(path) as f:
        lines = f.readlines()
        remove_indexes = []
        for (i, line) in enumerate(lines):
            if line.startswith("#include \""):
                remove_indexes.append(i)
        for i in reversed(remove_indexes):
            lines.pop(i)
    return lines

# Get content of a header file without license and header guard
def header_content(path: str, license_end_line: int = 29) -> str:
    return ''.join(lines_without_user_includes(path)[license_end_line + 1:])

# Get content of a source file without #include "..."
def source_content(path: str) -> str:
    return ''.join(lines_without_user_includes(path))

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

        dependencies = [filename]
        # Substitute includes
        for (i, line) in enumerate(lines):
            if line.startswith("#include \""):
                dependency = re.match("#include \"(.*)\"", line).group(1)
                dependencies.append(dependency.removeprefix("os/"))
                content = header_content("include/" + dependency)
                lines[i] = \
                f"// #include \"{dependency}\"\n" \
                 "// =========================\n" \
                f"{content}\n"                    \
                f"// End of   \"{dependency}\"\n" \
                 "// =========================\n" \
                 "\n"

        lines.append(
            "\n"                             \
            "\n"                             \
            "// -------------------------\n" \
            "// |        SOURCES        |\n" \
            "// -------------------------\n" \
            "\n"
        )

        for dependency in dependencies:
            if dependency in ignore or dependency == "os.hpp": continue

            src = dependency.removesuffix("pp").removesuffix("h") + "cpp"
            linux_path = "src/linux/" + src
            linux_content = source_content(linux_path)
            windows_path = "src/windows/" + src
            windows_content = source_content(windows_path)
            lines.append(
                 "#if IS_OS_LINUX\n"              \
                f"// {linux_path}\n"              \
                 "// =========================\n" \
                f"{linux_content}\n"              \
                f"// End of {linux_path}\n"       \
                 "// =========================\n" \
                 "\n"                             \
                 "#endif // IS_OS_LINUX\n"        \
                 "\n"                             \
                 "#if IS_OS_WINDOWS\n"            \
                f"// {windows_path}\n"            \
                 "// =========================\n" \
                f"{windows_content}\n"            \
                f"// End of {windows_path}\n"     \
                 "// =========================\n" \
                 "\n"                             \
                 "#endif // IS_OS_WINDOWS\n"
            )


        f.seek(0)
        f.truncate()
        f.writelines(lines)

print(filenames)