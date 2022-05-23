#
#	$Id: Makefile  2010-05-06 rh $
#
# GU behaviour interpreter Makefile
#
BIN=gubehaviourinterpreter

ALL_TARGETS=host 

CPP_SRCS=Activity.cpp Machine.cpp State.cpp MainStart.cpp ReadTransitionc.cpp Transition.cpp Posts.cpp SubMachine.cpp
HDRS=Activity.h Expression.h Machine.h State.h Transition.h FSMCodeType.h Posts.h OnePost.h SubMachine.h
C_SRCS=GUsem.c


.include "../../mk/whiteboard.mk"	# required for whiteboard clients
.include "../../mk/gunxtinterfacemodule.mk"	# required for whiteboard clients
.include "../../mk/util.mk"          # required for gu_util
#.include "../../mk/semaphore.mk"	# required for semaphore code
.include "../../mk/mipal.mk"	# comes last!
