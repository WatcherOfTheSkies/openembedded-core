LICENSE = "GPL-2.0-only"
LIC_FILES_CHKSUM = "file://COPYING;md5=b234ee4d69f5fce4486a80fdaf4a4263"

SRC_URI = "git://sourceware.org/git/systemtap.git;protocol=https;branch=master;tag=release-${PV} \
           file://0001-Do-not-let-configure-write-a-python-location-into-th.patch \
           file://0001-Install-python-modules-to-correct-library-dir.patch \
           file://0001-staprun-stapbpf-don-t-support-installing-a-non-root.patch \
           "

SRCREV = "c4fc655170b534478a6b131292b987630c7c0865"

COMPATIBLE_HOST = '(x86_64|i.86|powerpc|arm|aarch64|microblazeel|mips|riscv64).*-linux'
COMPATIBLE_HOST:libc-musl = 'null'

# systemtap can't be built without optimization, if someone tries to compile an
# entire image as -O0, break with fatal.
python () {
    if bb.utils.contains("SELECTED_OPTIMIZATION", "-O0", "x", "", d) == "x":
        bb.fatal("systemtap can't be built with -O0, using -O1 -Wno-error or -O1 instead.")
}
