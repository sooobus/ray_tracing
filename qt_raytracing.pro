 QT += widgets

  HEADERS       = \
    analogclock.h \
    geom.h \
    scene.h \
    utils.h \
    light.h
  SOURCES       = \
                  main.cpp \
    analogclock.cpp

  QMAKE_PROJECT_NAME = RayTracing

  # install
  target.path = $$[QT_INSTALL_EXAMPLES]/widgets/widgets/analogclock
  INSTALLS += target
