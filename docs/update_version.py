import re

# Extract version from file
with open("../include/os/libos.hpp", "r") as f:
    version = re.search("LIBOS_VERSION_STRING \"(.*)\"", f.read()).group(1)

print(version)

# Update Doxygen's version
with open("source/Doxyfile", "r+") as f:
    content = f.read()
    content = re.sub(
       "PROJECT_NUMBER         = .*",
       "PROJECT_NUMBER         = {}".format(version),
       content,
       re.MULTILINE
    )
    f.seek(0)
    f.truncate()
    f.write(content)


# Update Sphinx's version
with open("source/conf.py", "r+") as f:
    content = f.read()
    content = re.sub(
       "release = '.*'",
       "release = '{}'".format(version),
       content,
       re.MULTILINE
    )
    f.seek(0)
    f.truncate()
    f.write(content)