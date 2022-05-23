#
#	$Id: 8a9634944be7f33518663c9a51a1da0c5fc8fec5 $
#
# gu nxt make interface Makefile
#
#
BIN=gunxtnoisysensormodule

ALL_TARGETS=host 

CPP_SRCS=gunxtnoisysensormodule.cpp gunxtnoisegenerator.cpp boxmuller.cpp
C_SRCS=gunxtnoisysensor_wbmsg.c  
HDRS=gunxtnoisegenerator.h gunxtnoisysensor_wbmsg.h  boxmuller.h
NXT_WB_SRCS=gunxtbridge_wbmsg.c

.include "../../mk/whiteboard.mk" 
.include "../../mk/gunxtinterfacemodule.mk"	# required for whiteboard clients
.include "../../mk/mipal.mk"		# comes last!
