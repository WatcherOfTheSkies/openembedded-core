SUMMARY = "Linux kernel"
SECTION = "kernel"
LICENSE = "GPL-2.0-with-Linux-syscall-note"
HOMEPAGE = "https://www.yoctoproject.org/"

LIC_FILES_CHKSUM ?= "file://COPYING;md5=d7810fab7487fb0aad327b76f1be7cd7"

UPSTREAM_CHECK_GITTAGREGEX = "(?P<pver>\d+\.\d+(\.\d+)*)"

RECIPE_NO_UPDATE_REASON = "Recipe is updated through a separate process"

# Skip processing of this recipe if it is not explicitly specified as the
# PREFERRED_PROVIDER for virtual/kernel. This avoids network access required
# by the use of AUTOREV SRCREVs, which are the default for this recipe.
python () {
    if d.getVar("KERNEL_PACKAGE_NAME") == "kernel" and d.getVar("PREFERRED_PROVIDER_virtual/kernel") != d.getVar("PN"):
        d.delVar("BB_DONT_CACHE")
        raise bb.parse.SkipRecipe("Set PREFERRED_PROVIDER_virtual/kernel to %s to enable it" % (d.getVar("PN")))
}

DEPENDS += "xz-native bc-native"
DEPENDS:append:aarch64 = " libgcc"
KERNEL_CC:append:aarch64 = " ${TOOLCHAIN_OPTIONS}"
KERNEL_LD:append:aarch64 = " ${TOOLCHAIN_OPTIONS}"

DEPENDS:append:nios2 = " libgcc"
KERNEL_CC:append:nios2 = " ${TOOLCHAIN_OPTIONS}"
KERNEL_LD:append:nios2 = " ${TOOLCHAIN_OPTIONS}"

DEPENDS:append:arc = " libgcc"
KERNEL_CC:append:arc = " ${TOOLCHAIN_OPTIONS}"
KERNEL_LD:append:arc = " ${TOOLCHAIN_OPTIONS}"

KERNEL_FEATURES:append:qemuall = " features/debug/printk.scc features/taskstats/taskstats.scc"

KERNEL_FEATURES:append = " ${@bb.utils.contains('MACHINE_FEATURES', 'efi', 'cfg/efi.scc', '', d)}"
KERNEL_FEATURES:append = " ${@bb.utils.contains('MACHINE_FEATURES', 'numa', 'features/numa/numa.scc', '', d)}"
KERNEL_FEATURES:append = " ${@bb.utils.contains('MACHINE_FEATURES', 'vfat', 'cfg/fs/vfat.scc', '', d)}"

KERNEL_FEATURES_RISCV = "\
    arch/riscv/tunes/riscv-isa-clear.scc \
    ${@bb.utils.contains(    'TUNE_FEATURES', 'rv 32 i m a', 'arch/riscv/tunes/riscv-isa-rv32i.scc',  '', d)} \
    ${@bb.utils.contains(    'TUNE_FEATURES', 'rv 64 i m a', 'arch/riscv/tunes/riscv-isa-rv64i.scc',  '', d)} \
    ${@bb.utils.contains(    'TUNE_FEATURES', 'f d',         'arch/riscv/tunes/riscv-isa-fpu.scc',    '', d)} \
    ${@bb.utils.contains(    'TUNE_FEATURES', 'c',           'arch/riscv/tunes/riscv-isa-c.scc',      '', d)} \
    ${@bb.utils.contains(    'TUNE_FEATURES', 'v',           'arch/riscv/tunes/riscv-isa-v.scc',      '', d)} \
    ${@bb.utils.contains_any('TUNE_FEATURES', 'b zba',       'arch/riscv/tunes/riscv-isa-zba.scc',    '', d)} \
    ${@bb.utils.contains_any('TUNE_FEATURES', 'b zbb',       'arch/riscv/tunes/riscv-isa-zbb.scc',    '', d)} \
    ${@bb.utils.contains(    'TUNE_FEATURES', 'zbc',         'arch/riscv/tunes/riscv-isa-zbc.scc',    '', d)} \
    ${@bb.utils.contains(    'TUNE_FEATURES', 'zicbom',      'arch/riscv/tunes/riscv-isa-zicbom.scc', '', d)} \
    "

KERNEL_FEATURES:append:riscv32 = " ${KERNEL_FEATURES_RISCV}"
KERNEL_FEATURES:append:riscv64 = " ${KERNEL_FEATURES_RISCV}"

# A KMACHINE is the mapping of a yocto $MACHINE to what is built
# by the kernel. This is typically the branch that should be built,
# and it can be specific to the machine or shared
# KMACHINE = "UNDEFINED"

LINUX_VERSION_EXTENSION ??= "-yocto-${LINUX_KERNEL_TYPE}"

# Pick up shared functions
inherit kernel
inherit kernel-yocto

B = "${WORKDIR}/linux-${PACKAGE_ARCH}-${LINUX_KERNEL_TYPE}-build"

do_install:append(){
	if [ -n "${KMETA}" ]; then
		rm -rf ${STAGING_KERNEL_DIR}/${KMETA}
	fi
}

# enable kernel-sample for oeqa/runtime/cases's ksample.py test
KERNEL_FEATURES:append:qemuall = " features/kernel-sample/kernel-sample.scc"

KERNEL_DEBUG ?= ""
# These used to be version specific, but are now common dependencies.  New
# tools / dependencies will continue to be added in version specific recipes.
DEPENDS += '${@bb.utils.contains_any("ARCH", [ "x86", "arm64", "powerpc" ], "elfutils-native", "", d)}'
DEPENDS += "openssl-native util-linux-native"
DEPENDS += "gmp-native libmpc-native"

# Some options depend on CONFIG_PAHOLE_VERSION, so need to make pahole-native available before do_kernel_configme
do_kernel_configme[depends] += '${@bb.utils.contains("KERNEL_DEBUG", "True", "pahole-native:do_populate_sysroot", "", d)}'

EXTRA_OEMAKE += '${@bb.utils.contains("KERNEL_DEBUG", "True", "", "PAHOLE=false", d)}'

do_devshell:prepend() {
    # setup native pkg-config variables (kconfig scripts call pkg-config directly, cannot generically be overriden to pkg-config-native)
    d.setVar("PKG_CONFIG_DIR", "${STAGING_DIR_NATIVE}${libdir_native}/pkgconfig")
    d.setVar("PKG_CONFIG_PATH", "${PKG_CONFIG_DIR}:${STAGING_DATADIR_NATIVE}/pkgconfig")
    d.setVar("PKG_CONFIG_LIBDIR", "${PKG_CONFIG_DIR}")
    d.setVarFlag("PKG_CONFIG_SYSROOT_DIR", "unexport", "1")
    d.appendVar("OE_TERMINAL_EXPORTS", " PKG_CONFIG_DIR PKG_CONFIG_PATH PKG_CONFIG_LIBDIR PKG_CONFIG_SYSROOT_DIR")
}
