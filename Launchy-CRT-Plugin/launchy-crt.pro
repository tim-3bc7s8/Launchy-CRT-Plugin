 TEMPLATE      = lib
 CONFIG       += plugin debug_and_release
 FORMS		   = 
 HEADERS       = plugin_interface.h launchy_crt.h SessionManager.h gui.h
 SOURCES       = plugin_interface.cpp launchy_crt.cpp SessionManager.cpp gui.cpp 
 TARGET		   = launchy_crt
 
 win32 {
 	CONFIG -= embed_manifest_dll
	LIBS += shell32.lib
%	LIBS += user32.lib
%	LIBS += Gdi32.lib
%	LIBS += comctl32.lib
}

