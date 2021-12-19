Include(FetchContent)

FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG        v1.9.2)

FetchContent_MakeAvailable(spdlog)

include_directories(${spdlog_SOURCE_DIR}/include)
