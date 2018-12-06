AUTOMAKE_OPTIONS = foreign subdir-objects
AM_CXXFLAGS = -std=c++11 -Wall -O2 -ffunction-sections -fdata-sections -Wl,--gc-sections -std=c++11 -Iinclude -I$(top_srcdir)/include

plugins_dir=$(DESTDIR)@libdir@/libei2f/

#nobase_include_HEADERS = plugins/backend/Online.h \
#                         plugins/backend/PocoSQLite.h \
#                         plugins/backend/BerkeleyDB.h \
#                         plugins/backend/BloomFilter.h \
#                         plugins/backend/StdUnorderedSet.h \
#                         plugins/translator/SHA256.h \
#                         plugins/transport/HTTP.h \
#                         plugins/transport/File.h \
#                         plugins/updater/Line.h \
#                         plugins/updater/DifferentialLine.h \
#                         plugins/updater/DifferentialLineSeparated.h \
#                         plugins/updater/Binary.h

libei2f_la_SOURCES = src/helper.cpp \
                     src/patterns.cpp \
                     src/ObjectConfig.cpp \
                     src/pluginloader.cpp \
                     src/Stage.cpp \
                     src/ItemInfo.cpp \
                     src/PathMixin.cpp \
                     src/ListPipeline.cpp \
                     src/translator/Translator.cpp \
                     src/transport/Transport.cpp \
                     src/updater/Updater.cpp \
                     src/updater/ItemUpdater.cpp \
                     src/backend/Backend.cpp \
                     src/backend/ItemAPI.cpp \
                     src/backend/BinaryAPI.cpp \
                     plugins/backend/Online.h \
                     plugins/backend/PocoSQLite.h \
                     plugins/backend/BerkeleyDB.h \
                     plugins/backend/BloomFilter.h \
                     plugins/backend/StdUnorderedSet.h \
                     plugins/translator/SHA256.h \
                     plugins/transport/HTTP.h \
                     plugins/transport/File.h \
                     plugins/updater/Line.h \
                     plugins/updater/DifferentialLine.h \
                     plugins/updater/DifferentialLineSeparated.h \
                     plugins/updater/Binary.h

libei2f_la_LDFLAGS = -shared --gc-sections

libei2f_la_LIBADD = -lPocoFoundation

lib_LTLIBRARIES = libei2f.la \
                  libei2f.plugin.de.fraunhofer.fkie.ei2f.backend.Online.la \
                  libei2f.plugin.de.fraunhofer.fkie.ei2f.backend.PocoSQLite.la \
                  libei2f.plugin.de.fraunhofer.fkie.ei2f.backend.BerkeleyDB.la \
                  libei2f.plugin.de.fraunhofer.fkie.ei2f.backend.StdUnorderedSet.la \
                  libei2f.plugin.de.fraunhofer.fkie.ei2f.backend.BloomFilter.la \
                  libei2f.plugin.de.fraunhofer.fkie.ei2f.transport.File.la \
                  libei2f.plugin.de.fraunhofer.fkie.ei2f.transport.HTTP.la \
                  libei2f.plugin.de.fraunhofer.fkie.ei2f.translator.SHA256.la \
                  libei2f.plugin.de.fraunhofer.fkie.ei2f.updater.Binary.la \
                  libei2f.plugin.de.fraunhofer.fkie.ei2f.updater.Line.la \
                  libei2f.plugin.de.fraunhofer.fkie.ei2f.updater.DifferentialLine.la \
                  libei2f.plugin.de.fraunhofer.fkie.ei2f.updater.DifferentialLineSeparated.la

bin_PROGRAMS = test_BloomFilter \
               test_Backend_ReadIterator \
               test_string_replace \
               test_Online

libei2f_plugin_de_fraunhofer_fkie_ei2f_backend_Online_la_SOURCES = plugins/backend/Online.cpp
libei2f_plugin_de_fraunhofer_fkie_ei2f_backend_Online_la_LDFLAGS = -shared --gc-sections
libei2f_plugin_de_fraunhofer_fkie_ei2f_backend_Online_la_LIBADD = -lPocoFoundation libei2f.la

libei2f_plugin_de_fraunhofer_fkie_ei2f_backend_PocoSQLite_la_SOURCES = plugins/backend/PocoSQLite.cpp
libei2f_plugin_de_fraunhofer_fkie_ei2f_backend_PocoSQLite_la_LDFLAGS = -shared --gc-sections
libei2f_plugin_de_fraunhofer_fkie_ei2f_backend_PocoSQLite_la_LIBADD = libei2f.la -lPocoFoundation -lPocoData -lPocoDataSQLite

libei2f_plugin_de_fraunhofer_fkie_ei2f_backend_BerkeleyDB_la_SOURCES = plugins/backend/BerkeleyDB.cpp
libei2f_plugin_de_fraunhofer_fkie_ei2f_backend_BerkeleyDB_la_LDFLAGS = -shared --gc-sections
libei2f_plugin_de_fraunhofer_fkie_ei2f_backend_BerkeleyDB_la_LIBADD = libei2f.la -ldb

libei2f_plugin_de_fraunhofer_fkie_ei2f_backend_StdUnorderedSet_la_SOURCES = plugins/backend/StdUnorderedSet.cpp
libei2f_plugin_de_fraunhofer_fkie_ei2f_backend_StdUnorderedSet_la_LDFLAGS = -shared --gc-sections
libei2f_plugin_de_fraunhofer_fkie_ei2f_backend_StdUnorderedSet_la_LIBADD = libei2f.la

libei2f_plugin_de_fraunhofer_fkie_ei2f_backend_BloomFilter_la_SOURCES = plugins/backend/BloomFilter.cpp
libei2f_plugin_de_fraunhofer_fkie_ei2f_backend_BloomFilter_la_LDFLAGS = -shared --gc-sections
libei2f_plugin_de_fraunhofer_fkie_ei2f_backend_BloomFilter_la_LIBADD = libei2f.la

libei2f_plugin_de_fraunhofer_fkie_ei2f_transport_HTTP_la_SOURCES = plugins/transport/HTTP.cpp
libei2f_plugin_de_fraunhofer_fkie_ei2f_transport_HTTP_la_LDFLAGS = -shared --gc-sections
libei2f_plugin_de_fraunhofer_fkie_ei2f_transport_HTTP_la_LIBADD = libei2f.la -lPocoFoundation -lPocoNet -lPocoNetSSL

libei2f_plugin_de_fraunhofer_fkie_ei2f_transport_File_la_SOURCES = plugins/transport/File.cpp
libei2f_plugin_de_fraunhofer_fkie_ei2f_transport_File_la_LDFLAGS = -shared --gc-sections
libei2f_plugin_de_fraunhofer_fkie_ei2f_transport_File_la_LIBADD = libei2f.la

libei2f_plugin_de_fraunhofer_fkie_ei2f_translator_SHA256_la_SOURCES = plugins/translator/SHA256.cpp
libei2f_plugin_de_fraunhofer_fkie_ei2f_translator_SHA256_la_LDFLAGS = -shared --gc-sections
libei2f_plugin_de_fraunhofer_fkie_ei2f_translator_SHA256_la_LIBADD = libei2f.la -lPocoFoundation -lPocoCrypto

libei2f_plugin_de_fraunhofer_fkie_ei2f_updater_Binary_la_SOURCES = plugins/updater/Binary.cpp
libei2f_plugin_de_fraunhofer_fkie_ei2f_updater_Binary_la_LDFLAGS = -shared --gc-sections
libei2f_plugin_de_fraunhofer_fkie_ei2f_updater_Binary_la_LIBADD = libei2f.la

libei2f_plugin_de_fraunhofer_fkie_ei2f_updater_Line_la_SOURCES = plugins/updater/Line.cpp
libei2f_plugin_de_fraunhofer_fkie_ei2f_updater_Line_la_LDFLAGS = -shared --gc-sections
libei2f_plugin_de_fraunhofer_fkie_ei2f_updater_Line_la_LIBADD = libei2f.la

libei2f_plugin_de_fraunhofer_fkie_ei2f_updater_DifferentialLine_la_SOURCES = plugins/updater/DifferentialLine.cpp
libei2f_plugin_de_fraunhofer_fkie_ei2f_updater_DifferentialLine_la_LDFLAGS = -shared --gc-sections
libei2f_plugin_de_fraunhofer_fkie_ei2f_updater_DifferentialLine_la_LIBADD = libei2f.la libei2f.plugin.de.fraunhofer.fkie.ei2f.updater.Line.la

libei2f_plugin_de_fraunhofer_fkie_ei2f_updater_DifferentialLineSeparated_la_SOURCES = plugins/updater/DifferentialLineSeparated.cpp
libei2f_plugin_de_fraunhofer_fkie_ei2f_updater_DifferentialLineSeparated_la_LDFLAGS = -shared --gc-sections
libei2f_plugin_de_fraunhofer_fkie_ei2f_updater_DifferentialLineSeparated_la_LIBADD = libei2f.la libei2f.plugin.de.fraunhofer.fkie.ei2f.updater.Line.la

test_BloomFilter_SOURCES = test/test_BloomFilter.cpp
test_BloomFilter_LDADD = libei2f.la

test_Online_SOURCES = test/test_Online.cpp
test_Online_LDADD = libei2f.la

test_Backend_ReadIterator_SOURCES = test/test_Backend_ReadIterator.cpp
test_Backend_ReadIterator_LDADD = libei2f.la

test_string_replace_SOURCES = test/test_string_replace.cpp
test_string_replace_LDADD = libei2f.la

SUBDIRS = include

install-exec-hook:
	mkdir -p ${plugins_dir}
	cd ${plugins_dir} && ln -fs ../libei2f.plugin.de.fraunhofer.fkie.ei2f.backend.Online.so libei2f.plugin.de.fraunhofer.fkie.ei2f.backend.Online.so
	cd ${plugins_dir} && ln -fs ../libei2f.plugin.de.fraunhofer.fkie.ei2f.backend.PocoSQLite.so libei2f.plugin.de.fraunhofer.fkie.ei2f.backend.PocoSQLite.so
	cd ${plugins_dir} && ln -fs ../libei2f.plugin.de.fraunhofer.fkie.ei2f.backend.BerkeleyDB.so libei2f.plugin.de.fraunhofer.fkie.ei2f.backend.BerkeleyDB.so
	cd ${plugins_dir} && ln -fs ../libei2f.plugin.de.fraunhofer.fkie.ei2f.backend.StdUnorderedSet.so libei2f.plugin.de.fraunhofer.fkie.ei2f.backend.StdUnorderedSet.so
	cd ${plugins_dir} && ln -fs ../libei2f.plugin.de.fraunhofer.fkie.ei2f.backend.BloomFilter.so libei2f.plugin.de.fraunhofer.fkie.ei2f.backend.BloomFilter.so
	cd ${plugins_dir} && ln -fs ../libei2f.plugin.de.fraunhofer.fkie.ei2f.transport.HTTP.so libei2f.plugin.de.fraunhofer.fkie.ei2f.transport.HTTP.so
	cd ${plugins_dir} && ln -fs ../libei2f.plugin.de.fraunhofer.fkie.ei2f.transport.File.so libei2f.plugin.de.fraunhofer.fkie.ei2f.transport.File.so
	cd ${plugins_dir} && ln -fs ../libei2f.plugin.de.fraunhofer.fkie.ei2f.translator.SHA256.so libei2f.plugin.de.fraunhofer.fkie.ei2f.translator.SHA256.so
	cd ${plugins_dir} && ln -fs ../libei2f.plugin.de.fraunhofer.fkie.ei2f.updater.Binary.so libei2f.plugin.de.fraunhofer.fkie.ei2f.updater.Binary.so
	cd ${plugins_dir} && ln -fs ../libei2f.plugin.de.fraunhofer.fkie.ei2f.updater.Line.so libei2f.plugin.de.fraunhofer.fkie.ei2f.updater.Line.so
	cd ${plugins_dir} && ln -fs ../libei2f.plugin.de.fraunhofer.fkie.ei2f.updater.DifferentialLine.so libei2f.plugin.de.fraunhofer.fkie.ei2f.updater.DifferentialLine.so
	cd ${plugins_dir} && ln -fs ../libei2f.plugin.de.fraunhofer.fkie.ei2f.updater.DifferentialLineSeparated.so libei2f.plugin.de.fraunhofer.fkie.ei2f.updater.DifferentialLineSeparated.so

uninstall-hook:
	rm -rf ${plugins_dir}
