SUMMARY = "Python PE parsing module"
DESCRIPTION = "A multi-platform Python module to parse and work with Portable Executable (PE) files."
HOMEPAGE = "https://github.com/erocarrera/pefile"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://LICENSE;md5=e34c75178086aca0a17551ffbacaca53"

inherit setuptools3 ptest-python-pytest
SRCREV = "4b3b1e2e568a88d4f1897d694d684f23d9e270c4"
SRC_URI = "git://github.com/erocarrera/pefile;branch=master;protocol=https \
           file://run-ptest"

BBCLASSEXTEND = "native nativesdk"

RDEPENDS:${PN} += " \
    python3-mmap \
    python3-netclient \
    python3-stringold \
"
