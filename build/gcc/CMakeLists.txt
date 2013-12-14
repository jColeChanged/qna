# $Id: CMakeLists.txt 7588 2008-07-02 10:40:11Z gev $

PROJECT(libmessenger)

#Headers
INCLUDE_DIRECTORIES(	
    ../../
    )
    
ADD_DEFINITIONS(-D_GNU_SOURCE -pthread)

#Result
ADD_LIBRARY(version SHARED
    ../../bot_manager/bot_manager.cpp
    ../../bot_manager/bot_manager_item.cpp
    ../../bot_packet/bot_packet.cpp
    ../../xml_interface/xml_interface.cpp
    ../../sample.cpp
    ../../tinyxml/tinystr.cpp
    ../../tinyxml/tinyxml.cpp
    ../../tinyxml/tinyxmlerror.cpp
    ../../tinyxml/tinyxmlparser.cpp
    )