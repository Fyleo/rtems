ACLOCAL_AMFLAGS = -I ../aclocal

_SUBDIRS = hello capture ticker base_sp unlimited minimum fileio LIBGPIO_TEST LIBGPIO_TEST_IRQ LIBGPIO_JTAG SPI_23k256_TEST I2C_MCP23008_TEST

if MPTESTS
## base_mp is a sample multiprocessing test
_SUBDIRS += base_mp
endif

_SUBDIRS += paranoia

_SUBDIRS += nsecs

if CXXTESTS
_SUBDIRS += iostream cdtest
endif

if NETTESTS
## loopback tests a network loopback interface
_SUBDIRS += loopback
_SUBDIRS += pppd
endif

include $(top_srcdir)/../automake/test-subdirs.am
include $(top_srcdir)/../automake/local.am
