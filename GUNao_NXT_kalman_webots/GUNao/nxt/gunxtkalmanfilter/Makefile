#
#	$Id: 8a9634944be7f33518663c9a51a1da0c5fc8fec5 $
#
# gu nxt make interface Makefile
#
#
BIN=gunxtkalmanfilter

ALL_TARGETS=host 

CPP_SRCS=gunxtkalmanfiltermodule.cpp gunxtkalmanfilter.cpp kalman.cpp
C_SRCS=gunxtkalmanfilter_wbmsg.c  
HDRS=gunxtkalmanfilter.h gunxtkalmanfilter_wbmsg.h  kalman.h
NXT_WB_SRCS=gunxtbridge_wbmsg.c

.include "../../mk/whiteboard.mk" 
.include "../../mk/gunxtinterfacemodule.mk"	# required for whiteboard clients
.include "../../mk/mipal.mk"		# comes last!
