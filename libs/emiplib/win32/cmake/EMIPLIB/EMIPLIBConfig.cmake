
set(EMIPLIB_FOUND 1)


find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5OpenGL REQUIRED)

set(EMIPLIB_INCLUDE_DIRS  "C:/Users/Hugues/Desktop/CMakeInstallConfig/include" "D:/App/Qt/5.15.1/msvc2019_64/include/" "D:/App/Qt/5.15.1/msvc2019_64/include/QtGui" "D:/App/Qt/5.15.1/msvc2019_64/include/QtCore" "D:/App/Qt/5.15.1/msvc2019_64/.//mkspecs/win32-msvc" "D:/App/Qt/5.15.1/msvc2019_64/include//QtANGLE" "D:/App/Qt/5.15.1/msvc2019_64/include/QtWidgets" "D:/App/Qt/5.15.1/msvc2019_64/include/QtOpenGL")

set(EMIPLIB_LIBRARIES  "optimized" "C:/Users/Hugues/Desktop/CMakeInstallConfig/lib/emiplib.lib" "debug" "C:/Users/Hugues/Desktop/CMakeInstallConfig/lib/emiplib_d.lib" "optimized" "C:/Users/Hugues/Desktop/CMakeInstallConfig/lib/jthread.lib" "debug" "C:/Users/Hugues/Desktop/CMakeInstallConfig/lib/jthread_d.lib" "optimized" "C:/Users/Hugues/Desktop/CMakeInstallConfig/lib/jrtplib.lib" "debug" "C:/Users/Hugues/Desktop/CMakeInstallConfig/lib/jrtplib_d.lib" "optimized" "C:/Users/Hugues/Desktop/CMakeInstallConfig/lib/jthread.lib" "debug" "C:/Users/Hugues/Desktop/CMakeInstallConfig/lib/jthread_d.lib" "ws2_32" "winmm" "Qt5::Gui" "Qt5::Widgets" "Qt5::OpenGL")

