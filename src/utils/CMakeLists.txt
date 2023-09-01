add_library(utils 
            ipcconnect.cpp 
            tev-display.cpp 
            ipcimpl_linux.inl 
            ipcimpl_win.inl
            )
target_compile_definitions(utils PUBLIC ${DEFINITIONS})
message("util definitions: ${DEFINITIONS}")
target_include_directories(utils PUBLIC ${CMAKE_CURRENT_LIST_DIR}/)
