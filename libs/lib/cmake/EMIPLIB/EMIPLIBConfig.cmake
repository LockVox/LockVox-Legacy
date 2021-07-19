
set(EMIPLIB_FOUND 1)


find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5OpenGL REQUIRED)

set(EMIPLIB_INCLUDE_DIRS  "D:/Libs/include" "D:/App/Qt/5.15.2/msvc2019_64/include/" "D:/App/Qt/5.15.2/msvc2019_64/include/QtGui" "D:/App/Qt/5.15.2/msvc2019_64/include/QtCore" "D:/App/Qt/5.15.2/msvc2019_64/.//mkspecs/win32-msvc" "D:/App/Qt/5.15.2/msvc2019_64/include//QtANGLE" "D:/App/Qt/5.15.2/msvc2019_64/include/QtWidgets" "D:/App/Qt/5.15.2/msvc2019_64/include/QtOpenGL")

set(EMIPLIB_LIBRARIES  "optimized" "D:/Libs/lib/emiplib.lib" "debug" "D:/Libs/lib/emiplib_d.lib" "optimized" "D:/Libs/lib/jthread.lib" "debug" "D:/Libs/lib/jthread_d.lib" "optimized" "D:/Libs/lib/jrtplib.lib" "debug" "D:/Libs/lib/jrtplib_d.lib" "optimized" "D:/Libs/lib/jthread.lib" "debug" "D:/Libs/lib/jthread_d.lib" "ws2_32" "D:/Libs/lib/portaudio_x64.lib" "Qt5::Gui" "Qt5::Widgets" "Qt5::OpenGL")

