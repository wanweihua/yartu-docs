INCLUDEPATH += $$PWD/boost_1_58_0

CORE_BOOST_LIBS = $$PWD/boost_1_58_0/build/$$CORE_BUILDS_PLATFORM_PREFIX/static

core_boost_fpic {
    CORE_BOOST_LIBS = $$PWD/boost_1_58_0/build/$$CORE_BUILDS_PLATFORM_PREFIX/static_fpic
}
core_boost_shared {
    CORE_BOOST_LIBS = $$PWD/boost_1_58_0/build/$$CORE_BUILDS_PLATFORM_PREFIX/shared
}

core_boost_libs {

core_windows {
core_debug {
    LIBS += -L$$CORE_BOOST_LIBS -llibboost_system-vc120-mt-gd-1_58 -llibboost_filesystem-vc120-mt-gd-1_58
}
core_release {
    LIBS += -L$$CORE_BOOST_LIBS -llibboost_system-vc120-mt-1_58 -llibboost_filesystem-vc120-mt-1_58
}
} else {
    LIBS += -L$$CORE_BOOST_LIBS -lboost_system -lboost_filesystem
}

}

core_boost_regex {

core_windows {
core_debug {
    LIBS += -L$$CORE_BOOST_LIBS -llibboost_regex-vc120-mt-gd-1_58
}
core_release {
    LIBS += -L$$CORE_BOOST_LIBS -llibboost_regex-vc120-mt-1_58
}
} else {
    LIBS += -L$$CORE_BOOST_LIBS -lboost_regex
}

}
