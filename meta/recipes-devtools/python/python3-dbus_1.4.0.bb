SUMMARY = "Python bindings for the DBus inter-process communication system"
SECTION = "devel/python"
HOMEPAGE = "http://www.freedesktop.org/Software/dbus"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://COPYING;md5=97f58951300aa52a9f9e3a62bd5c846c"
DEPENDS = "expat dbus glib-2.0 virtual/libintl"

SRC_URI = "http://dbus.freedesktop.org/releases/dbus-python/dbus-python-${PV}.tar.xz"

SRC_URI[sha256sum] = "c36b28f10ffcc8f1f798aca973bcc132f91f33eb9b6b8904381b4077766043d5"

S = "${UNPACKDIR}/dbus-python-${PV}"

inherit setuptools3-base meson pkgconfig

# requires dbus-run-session
EXTRA_OEMESON += "-Dtests=false"

RDEPENDS:${PN} = "python3-io python3-logging python3-stringold python3-threading python3-xml"

FILES:${PN}-dev += "${libdir}/pkgconfig"

BBCLASSEXTEND = "native nativesdk"
