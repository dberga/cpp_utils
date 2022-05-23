PORTSDIR?=/opt/local
SOAPDIR?=${PORTSDIR}/share/gsoap/import
SOAPCXX?=${PORTSDIR}/bin/soapcpp2
.if exists(${AL_DIR}/lib/libalsoap.a)
SOAPALIB+=libalsoap.a
.else
SOAPALIB+=liblibsoap.a
.endif
SOAPALIB+=libtinyxml.a
.ifndef SOAPLIB
.for alib in ${SOAPALIB}
SOAPLIB+=${AL_DIR}/lib/${alib}
.endfor
.endif
CORESERVICES?=-framework CoreServices
ZEROCONF_LIBS?=${CORESERVICES}
JPEG_LIBS?=-framework libjpeg
DISPATCH_LIBS?=
OUTFILE=${BIN}
OUTFILE_LOCAL?=lib${BIN}.dylib
OUT=${OUTFILE${LOCAL}}
OUTPATH=${BUILDDIR}/${OUT}
COMMON_OFLAGS=-o ${OUTPATH}
COMMON_ALOFLAGS+=-Wl,-rpath,${AL_DIR}/lib ${COMMON_OFLAGS}
COMMON_ALLDFLAGS=
HOST_ARCH?=-m32 -arch i386
HOST_CPPFLAGS?=-I${PORTSDIR}/include
HOST_SOAP_CFLAGS?=${HOST_ARCH}
HOST_SOAP_LDFLAGS?=${HOST_ARCH} -L${AL_DIR}/lib -headerpad_max_install_names \
		-Wl,-search_paths_first
HOST_ALCPPFLAGS?=-DNO_GLOBAL_NAMESPACES -DTIXML_USE_STL -DTIXML_USE_STL	\
		 -DTIXML_USE_STL -DTIXML_USE_STL
HOST_OUTFLAGS?=${COMMON_OFLAGS}
HOST_LDFLAGS?=-L${PORTSDIR}/lib
HOST_ALCFLAGS?=
HOST_ALLDFLAGS?=${COMMON_ALLDFLAGS}
HOST_ALALIBS+=libalcommon.a
.if exists(${AL_DIR}/lib/libalippc.a)
HOST_ALALIBS+=libalippc.a
.else
HOST_ALALIBS+=libippc.a
.endif
HOST_ALALIBS+=libboost_serialization-xgcc40-mt-1_38.a
HOST_ALALIBS+=libboost_system-xgcc40-mt-1_38.a
HOST_ALALIBS+=libboost_date_time-xgcc40-mt-1_38.a
HOST_ALALIBS+=libboost_thread-xgcc40-mt-1_38.a
HOST_ALALIBS+=libalvalue.a
.if exists(${AL_DIR}/lib/libalcore.dylib)
HOST_ALALIBS+=libalcore.dylib 
.else
HOST_ALALIBS+=liblibcore.a
.endif
.if exists(${AL_DIR}/lib/liballog.a)
HOST_ALALIBS+=liballog.a
.endif
HOST_ALALIBS+=libboost_signals-xgcc40-mt-1_38.a
.if exists(${AL_DIR}/lib/libaltools.a)
HOST_ALALIBS+=libaltools.a
.else
HOST_ALALIBS+=libtools.a
.endif
HOST_ALALIBS+=libboost_system-xgcc40-mt-1_38.a
HOST_ALALIBS+=libboost_filesystem-xgcc40-mt-1_38.a
.if exists(${AL_DIR}/lib/libalfindippc.a)
HOST_ALALIBS+=libalfindippc.a
.else
HOST_ALALIBS+=libfindippc.a
.endif
HOST_ALALIBS+=librttools.a
.if exists(${AL_DIR}/lib/libalthread.a)
HOST_ALALIBS+=libalthread.a
.else
HOST_ALALIBS+=liblibthread.a
.endif
.if exists(${AL_DIR}/lib/libalfile.a)
HOST_ALALIBS+=libalfile.a
.endif
HOST_ALALIBS+=libboost_program_options-xgcc40-mt-1_38.a
#HOST_ALALIBS+=python2.5/config/libpython2.5.a
HOST_ALALIBS+=libboost_date_time-xgcc40-mt-1_38.a
HOST_ALALIBS+=libboost_thread-xgcc40-mt-1_38.a
HOST_ALALIBS+=libalvalue.a
#HOST_ALALIBS+=liblibcore.a
HOST_ALALIBS+=libboost_signals-xgcc40-mt-1_38.a
#HOST_ALALIBS+=libtools.a
HOST_ALALIBS+=libboost_filesystem-xgcc40-mt-1_38.a
#HOST_ALALIBS+=libfindippc.a
HOST_ALALIBS+=${SOAPALIB}
HOST_ALALIBS+=librttools.a
#HOST_ALALIBS+=liblibthread.a
HOST_ALALIBS+=libboost_program_options-xgcc40-mt-1_38.a
.if exists(${AL_DIR}/lib/libalfactory.a)
HOST_ALALIBS+=libalfactory.a
.else
HOST_ALALIBS+=liblibfactory-static.a
.endif
.ifndef HOST_ALLIBS
HOST_ALLIBS=-Wl,-rpath,${AL_DIR}/lib
.for alib in ${HOST_ALALIBS}
HOST_ALLIBS+=${AL_DIR}/lib/${alib}
.endfor
HOST_ALLIBS+=/usr/lib/libdl.dylib
HOST_ALLIBS+=/usr/lib/python2.5/config/libpython2.5.a
.endif
HOST_LOCAL_ALCPPFLAGS?=${HOST_ALCPPFLAGS} -D${BIN}_EXPORTS
HOST_LOCAL_ALCFLAGS?=${HOST_ALCFLAGS} -fPIC
HOST_LOCAL_ALLDFLAGS?=${HOST_ARCH} -L${AL_DIR}/lib			\
		      -headerpad_max_install_names -Wl,-search_paths_first \
		      ${COMMON_ALLDFLAGS}
HOST_LOCAL_ALLIBS?=${HOST_ALLIBS}
HOST_LOCAL_ALOFLAGS?=${COMMON_ALOFLAGS}
HOST_LOCAL_OUTFLAGS?=-dynamiclib -Wl,-flat_namespace			\
		     -install_name,${AL_DIR}/lib/${OUTFILE_LOCAL}	\
		     ${COMMON_OFLAGS}
.ifndef HOST_GCC
HOST_GCC!=which {{,/Volumes/*}/Xcode4*/usr/{local/,}bin/,}gcc 2>/dev/null | head -n1
.endif
.ifndef HOST_CC
.  ifdef PROFILING
HOST_CC=${HOST_GCC}
.  else
HOST_CC!=which {{,/Volumes/*}/Xcode4*/usr/{local/,}bin/,}clang gcc 2>/dev/null | head -n1
.  endif
.endif
.ifndef HOST_GXX
HOST_GXX!=which {/{,Volumes/*/}Xcode4*/usr/{local/,}bin/,}g++ 2>/dev/null | head -n1
.endif
.ifndef HOST_CXX
.  ifdef PROFILING
HOST_CXX=${HOST_GXX}
.  else
HOST_CXX!=which {/{,Volumes/*/}Xcode4*/usr/{local/,}bin/,}clang++ g++ 2>/dev/null | head -n1
.  endif
.endif

HOST_PROFILE_FLAGS?=-finstrument-functions -pg
HOST_PROFILE_CFLAGS?=${HOST_ARCH} ${HOST_PROFILE_FLAGS}
HOST_PROFILE_LDFLAGS?=${HOST_ARCH} ${HOST_PROFILE_FLAGS} -lSaturn

#LINK_FIXUP?=install_name_tool -change libalcore.dylib @rpath/libalcore.dylib ${OUTPATH}
