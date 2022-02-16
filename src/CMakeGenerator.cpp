#include "CMakeGenerator.hpp"

void UBT::generateCmake(const char* name, const CMakeInfoData& data)
{
    auto stream = std::ofstream(path + static_cast<std::string>("CMakeLists.txt"));
    stream << R"(
cmake_minimum_required(VERSION 3.19)

if (APPLE)
    message(FATAL_ERROR "You're currently working on an Apple machine! Apple and their platforms are not supported, if
you wish to help with adding support for apple devices, please do a pull request on the main engine repository as well
as on the UVKBuildTool repository(this file is generated by it!) and consider maintaining the apple builds!")
endif()
project(UntitledVulkanGameEngine)
project(Modlib)
)" << std::endl;
    stream << "project(" << name << "Modded)" << std::endl;
    stream << "project(" << name << ")" << std::endl;

    // Basically everything that can be directly copy/pasted
    stream << R"(
set(CMAKE_CXX_STANDARD 20)

# Adding subdirectories so that we can consume the libraries
add_subdirectory(Engine/ThirdParty/glm/)
add_subdirectory(Engine/ThirdParty/glfw/)
add_subdirectory(Engine/ThirdParty/vulkan/headers/)
add_subdirectory(Engine/ThirdParty/glew/)
add_subdirectory(Engine/ThirdParty/entt/)
add_subdirectory(Engine/ThirdParty/assimp)

set(BUILD_SHARED_LIBS ON)
add_subdirectory(Engine/ThirdParty/yaml/)
add_subdirectory(Engine/ThirdParty/freetype)
set(BUILD_SHARED_LIBS OFF)
if(MINGW)
else()
    add_subdirectory(Engine/ThirdParty/openal/)
endif()

# Added linking directories because we're using out custom ones and we don't want to pollute path
link_directories(Engine/ThirdParty/vulkan/)
link_directories(Engine/ThirdParty/vulkan/lib)
link_directories(Engine/ThirdParty/glew/lib)

# Include directories for everything really
include_directories(Engine/ThirdParty/assimp/include)
include_directories(.)
include_directories(Engine/)
include_directories(Engine/ThirdParty/entt/src/entt)
include_directories(Engine/Core/)
include_directories(Engine/ThirdParty/glm)
include_directories(Engine/ThirdParty/)
include_directories(Engine/ThirdParty/imguiex/imguizmo)
include_directories(Engine/ThirdParty/glew/include)
include_directories(Engine/ThirdParty/yaml/include)
include_directories(Engine/ThirdParty/yaml/include/yaml-cpp)
include_directories(Engine/ThirdParty/logger/)
include_directories(Engine/ThirdParty/urll/)
include_directories(Source/)
include_directories(Engine/ThirdParty/imgui/backends/everything/)
include_directories(Engine/ThirdParty/imgui/backends/Vulkan/)
include_directories(Engine/ThirdParty/imgui/misc/)
include_directories(Engine/ThirdParty/glfw/include/GLFW/)
include_directories(Engine/ThirdParty/vulkan/headers/include)
include_directories(Engine/ThirdParty/freetype/include)
if(WIN32)
    include_directories(Engine/ThirdParty/stb)
)";
    for (const auto& a : data.msvcIncludePaths)
        stream << "    include_directories(" << a << ")\n";
    for (const auto& a : data.msvcSubdirectories)
        stream << "    add_subdirectory(" << a << ")\n";
    stream << R"(
else()
    include_directories(Engine/ThirdParty)
)";
    for (const auto& a : data.unixIncludePaths)
        stream << "    include_directories(" << a << ")\n";
    for (const auto& a : data.unixSubdirectories)
        stream << "    add_subdirectory(" << a << ")\n";
    stream << R"(
endif()
file(GLOB_RECURSE )" << name << R"(Headers "Source/*.hpp")

# Source code of the engine
if(WIN32)
    file(GLOB_RECURSE EngineSrc "Engine/Audio/*.cpp" "Engine/Audio/*.hpp" "Engine/Core/*.cpp" "Engine/Core/*.hpp"
        "Engine/Renderer/*.hpp" "Engine/Renderer/*.cpp" "Engine/GameFramework/*.cpp" "Engine/GameFramework/*.hpp"
        "Engine/ThirdParty/imgui/backends/everything/*.cpp" "Engine/ThirdParty/imgui/misc/*.cpp" "Engine/ThirdParty/imgui/misc/*.h"
        "Engine/ThirdParty/imgui/backends/everything/*.h" "Engine/ThirdParty/imgui/backends/Vulkan/*.h" "Engine/ThirdParty/imgui/backends/Vulkan/*.cpp"
        "Engine/ThirdParty/imguiex/*.cpp" "Engine/ThirdParty/imguiex/*.h" "Generated/*.hpp" "Engine/ThirdParty/logger/*.h"
        "Engine/ThirdParty/logger/*.cpp" )";
    addFilesToStream(stream, data.msvcSources, LIB_FLAGS_LINK_TO_ENGINE);
    stream << R"()
    # The headers of the engine library
    file(GLOB_RECURSE EngineHeaders "Engine/Audio/*.hpp" "Engine/Core/*.hpp" "Engine/Renderer/*.hpp" "Engine/GameFramework/*.hpp"
        "Engine/GameFramework/*.hpp" "Engine/ThirdParty/imgui/misc/*.h" "Engine/ThirdParty/imgui/backends/everything/*.h"
        "Engine/ThirdParty/imgui/backends/Vulkan/*.h" "Engine/ThirdParty/imguiex/*.h" "Generated/*.hpp"
        "Engine/ThirdParty/logger/*.h" )";
    addFilesToStream(stream, data.msvcHeaders, LIB_FLAGS_LINK_TO_ENGINE);
    stream << R"()
    file(GLOB_RECURSE ExecutableSrc "WrapperSource/*.cpp" "WrapperSource/*.hpp" )";
    addFilesToStream(stream, data.msvcSources, LIB_FLAGS_LINK_TO_WRAPPER);
    addFilesToStream(stream, data.msvcHeaders, LIB_FLAGS_LINK_TO_WRAPPER);
    stream << R"()
else()
    file(GLOB_RECURSE EngineSrc "Engine/Audio/*.cpp" "Engine/Audio/*.hpp" "Engine/Core/*.cpp" "Engine/Core/*.hpp"
        "Engine/Renderer/*.hpp" "Engine/Renderer/*.cpp" "Engine/GameFramework/*.cpp" "Engine/GameFramework/*.hpp"
        "Engine/ThirdParty/imgui/backends/everything/*.cpp" "Engine/ThirdParty/imgui/misc/*.cpp" "Engine/ThirdParty/imgui/misc/*.h"
        "Engine/ThirdParty/imgui/backends/everything/*.h" "Engine/ThirdParty/imgui/backends/Vulkan/*.h" "Engine/ThirdParty/imgui/backends/Vulkan/*.cpp"
        "Engine/ThirdParty/imguiex/*.cpp" "Engine/ThirdParty/imguiex/*.h" "Generated/*.hpp" "Engine/ThirdParty/logger/*.h"
        "Engine/ThirdParty/logger/*.cpp" )";
    addFilesToStream(stream, data.unixSources, LIB_FLAGS_LINK_TO_ENGINE);
    stream << R"()
    # The headers of the engine library
    file(GLOB_RECURSE EngineHeaders "Engine/Audio/*.hpp" "Engine/Core/*.hpp" "Engine/Renderer/*.hpp" "Engine/GameFramework/*.hpp"
        "Engine/GameFramework/*.hpp" "Engine/ThirdParty/imgui/misc/*.h" "Engine/ThirdParty/imgui/backends/everything/*.h"
        "Engine/ThirdParty/imgui/backends/Vulkan/*.h" "Engine/ThirdParty/imguiex/*.h" "Generated/*.hpp"
        "Engine/ThirdParty/logger/*.h" )";
    addFilesToStream(stream, data.unixHeaders, LIB_FLAGS_LINK_TO_ENGINE);
    stream << R"()
    file(GLOB_RECURSE ExecutableSrc "WrapperSource/*.cpp" "WrapperSource/*.hpp" )";
    addFilesToStream(stream, data.unixSources, LIB_FLAGS_LINK_TO_WRAPPER);
    addFilesToStream(stream, data.unixHeaders, LIB_FLAGS_LINK_TO_WRAPPER);
    stream << R"()
endif()

# Headers to precompile
file(GLOB_RECURSE PrecompileEngineHeaders "Engine/Core/Core/STL.hpp" "Engine/ThirdParty/glm/glm/glm.hpp"
"Engine/ThirdParty/stb/*.h" "Engine/ThirdParty/yaml/include/yaml-cpp/*.h" "Engine/ThirdParty/glm/glm/*.h"
"Engine/ThirdParty/logger/UVKLog.h" "Engine/ThirdParty/yaml/include/yaml-cpp/*.hpp"
"Engine/ThirdParty/entt/*.h" "Engine/ThirdParty/freetype/include/freetype/freetype.h"
"Engine/ThirdParty/vulkan/headers/include/vulkan/vulkan.h")
)" << std::endl;
    stream << R"(if(WIN32)
    file(GLOB_RECURSE )" << name << R"(Src "Source/*.hpp" "Source/*.cpp" )";
    stream << R"()
    add_library(UntitledVulkanGameEngine SHARED Engine/ThirdParty/stb/stb_image.h Engine/ThirdParty/stb/sndfile.h ${EngineSrc} ${)" << name << R"(Src})
else()
    file(GLOB_RECURSE )" << name << R"(Src "Source/*.hpp" "Source/*.cpp" )";
    stream << R"()
    add_library(UntitledVulkanGameEngine SHARED Engine/ThirdParty/stb/stb_image.h ${EngineSrc} ${)" << name << R"(Src})
endif()

add_library(Modlib SHARED Generated/ModEmpty.cpp Generated/ModEmpty.hpp)

# Add the executable for the normal and modded game)" << std::endl;
    stream << "add_executable(" << name << R"( ${ExecutableSrc} "Generated/main.cpp" "Engine/ThirdParty/urll/urll.h" "Engine/ThirdParty/urll/urll.cpp" ${)" << name << R"(Headers})
if(WIN32)
    add_executable()" << name << R"(Modded ${ExecutableSrc} "Generated/mainmodded.cpp" "Engine/ThirdParty/urll/urll.h" "Engine/ThirdParty/urll/urll.cpp" ${)" << name << R"(Headers} )";
    addFilesToStream(stream, data.msvcHeaders, LIB_FLAGS_LINK_TO_WRAPPER_MODDED);
    addFilesToStream(stream, data.msvcSources, LIB_FLAGS_LINK_TO_WRAPPER_MODDED);
    stream << R"()
else()
    add_executable()" << name << R"(Modded ${ExecutableSrc} "Generated/mainmodded.cpp" "Engine/ThirdParty/urll/urll.h" "Engine/ThirdParty/urll/urll.cpp" ${)" << name << R"(Headers} )";
    addFilesToStream(stream, data.unixHeaders, LIB_FLAGS_LINK_TO_WRAPPER_MODDED);
    addFilesToStream(stream, data.unixSources, LIB_FLAGS_LINK_TO_WRAPPER_MODDED);
    stream << R"()
endif()

target_precompile_headers(UntitledVulkanGameEngine PRIVATE ${PrecompileEngineHeaders})
set_target_properties(UntitledVulkanGameEngine PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(Modlib PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties()" << name << R"( PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties()" << name << R"(Modded PROPERTIES LINKER_LANGUAGE CXX))" << std::endl;
    stream << "target_compile_definitions(" << name << R"(Modded PRIVATE "UVK_COMPILING_WITH_MODS" "URLL_USE_FUNCTIONAL")
target_compile_definitions(UntitledVulkanGameEngine PRIVATE "UVK_LIB_COMPILE" "YAML_CPP_DLL")

if (WIN32)
    if (MINGW)
        target_compile_options(UntitledVulkanGameEngine PRIVATE "-O3" "-march=native")
        target_link_libraries(UntitledVulkanGameEngine glfw opengl32 libglew_static yaml-cpp vulkan-1 assimp freetype)

        target_compile_options(Modlib PRIVATE "-O3" "-march=native")
        target_link_libraries(Modlib)

        target_compile_options()" << name << R"( PRIVATE "-O3" "-march=native")
        target_link_libraries()" << name << R"( UntitledVulkanGameEngine)

        target_compile_options()" << name << R"(Modded PRIVATE "-O3" "-march=native")
        target_link_libraries()" << name << R"(Modded )" << R"(UntitledVulkanGameEngine)
    else()
        target_compile_options(UntitledVulkanGameEngine PRIVATE "/O2" "/MP" "/Ob2" "/Oi" "/Ot" "/arch:AVX2")
        target_link_libraries(UntitledVulkanGameEngine glfw OpenAL opengl32 libglew_static yaml-cpp vulkan-1 sndfile assimp freetype )";
        for (const auto& a : data.msvcLinkLibraries)
        {
            if (a.target & LIB_FLAGS_LINK_TO_ENGINE)
            {
                stream << a.val << " ";
            }
        }
        stream << R"()

        target_compile_options(Modlib PRIVATE "/O2" "/MP" "/Ob2" "/Oi" "/Ot" "/arch:AVX2")
        target_link_libraries(Modlib)

        target_compile_options()" << name << R"( PRIVATE "/O2" "/Ob2" "/Oi" "/Ot" "/arch:AVX2")
        target_link_libraries()" << name << R"( UntitledVulkanGameEngine )";
        for (const auto& a : data.msvcLinkLibraries)
        {
            if (a.target & LIB_FLAGS_LINK_TO_WRAPPER)
            {
                stream << a.val << " ";
            }
        }
        stream << R"()

        target_compile_options()" << name << R"(Modded PRIVATE "/O2" "/MP" "/Ob2" "/Oi" "/Ot" "/arch:AVX2")
        target_link_libraries()" << name << "Modded " << R"(UntitledVulkanGameEngine )";
        for (const auto& a : data.msvcLinkLibraries)
        {
            if (a.target & LIB_FLAGS_LINK_TO_WRAPPER_MODDED)
            {
                stream << a.val << " ";
            }
        }
        stream << R"()
    endif()
else()
    target_compile_options(UntitledVulkanGameEngine PRIVATE "-O3" "-march=native")
    target_link_libraries(UntitledVulkanGameEngine glfw GLEW OpenGL OpenAL yaml-cpp vulkan sndfile util pthread assimp freetype )";
    for (const auto& a : data.unixLinkLibraries)
    {
        if (a.target & LIB_FLAGS_LINK_TO_ENGINE)
        {
            stream << a.val << " ";
        }
    }
    stream << R"()

    target_compile_options(Modlib PRIVATE "-O3" "-march=native")
    target_link_libraries(Modlib)

    target_compile_options()" << name << R"( PRIVATE "-O3" "-march=native")
    target_link_libraries()" << name << R"( UntitledVulkanGameEngine )";
    for (const auto& a : data.unixLinkLibraries)
    {
        if (a.target & LIB_FLAGS_LINK_TO_WRAPPER)
        {
            stream << a.val << " ";
        }
    }
    stream << R"()

    target_compile_options()" << name << R"(Modded PRIVATE "-O3" "-march=native" )
    target_link_libraries()" << name << "Modded" << R"( UntitledVulkanGameEngine dl )";
    for (const auto& a : data.unixLinkLibraries)
    {
        if (a.target & LIB_FLAGS_LINK_TO_WRAPPER_MODDED)
        {
            stream << a.val << " ";
        }
    }
    stream << R"()
endif())" << std::endl;

    stream.close();
}

void UBT::addHeaderLibraries(YAML::Node& config, CMakeInfoData& data)
{
    if (config["additional-header-libraries"])
    {
        auto includes = config["additional-header-libraries"];
        if (includes["msvc"])
        {
            auto target = includes["msvc"];
            data.msvcHeaders.push_back({
                .prjtype = LIB_FLAGS_NONE,
            });

            if (target["engine"])
            {
                data.msvcHeaders.back().prjtype = static_cast<LibraryProjectType>(data.msvcHeaders.back().prjtype | LIB_FLAGS_LINK_TO_ENGINE);
                auto engine = target["engine"];
                accumulateHeaderLibraries(engine, data.msvcHeaders);
            }
            if (target["wrapper"])
            {
                data.msvcHeaders.back().prjtype = static_cast<LibraryProjectType>(data.msvcHeaders.back().prjtype | LIB_FLAGS_LINK_TO_WRAPPER);
                auto engine = target["wrapper"];
                accumulateHeaderLibraries(engine, data.msvcHeaders);
            }
            if (target["modded-wrapper"])
            {
                data.msvcHeaders.back().prjtype = static_cast<LibraryProjectType>(data.msvcHeaders.back().prjtype | LIB_FLAGS_LINK_TO_WRAPPER_MODDED);
                auto engine = target["modded-wrapper"];
                accumulateHeaderLibraries(engine, data.msvcHeaders);
            }
        }
        if (includes["unix"])
        {
            auto target = includes["unix"];
            data.unixHeaders.push_back({
                .prjtype = LIB_FLAGS_NONE,
            });

            if (target["engine"])
            {
                data.unixHeaders.back().prjtype = static_cast<LibraryProjectType>(data.unixHeaders.back().prjtype | LIB_FLAGS_LINK_TO_ENGINE);
                auto engine = target["engine"];
                accumulateHeaderLibraries(engine, data.unixHeaders);
            }
            if (target["wrapper"])
            {
                data.unixHeaders.back().prjtype = static_cast<LibraryProjectType>(data.unixHeaders.back().prjtype | LIB_FLAGS_LINK_TO_WRAPPER);
                auto engine = target["wrapper"];
                accumulateHeaderLibraries(engine, data.unixHeaders);
            }
            if (target["modded-wrapper"])
            {
                data.unixHeaders.back().prjtype = static_cast<LibraryProjectType>(data.unixHeaders.back().prjtype | LIB_FLAGS_LINK_TO_WRAPPER_MODDED);
                auto engine = target["modded-wrapper"];
                accumulateHeaderLibraries(engine, data.unixHeaders);
            }
        }
    }
}

void UBT::addIncludeDirectories(YAML::Node& config, CMakeInfoData& data)
{
    if (config["additional-include-directories"])
    {
        auto includes = config["additional-include-directories"];
        if (includes["msvc"])
            for (const auto& a : includes["msvc"])
                data.msvcIncludePaths.push_back(a.as<std::string>());
        if (includes["unix"])
            for (const auto& a : includes["unix"])
                data.unixIncludePaths.push_back(a.as<std::string>());
    }
}

void UBT::addLinkLibraries(YAML::Node& config, CMakeInfoData& data)
{
    if (config["additional-link-libraries"])
    {
        auto ll = config["additional-link-libraries"];
        if (ll["msvc"])
        {
            for (const auto& target : ll["msvc"])
            {
                UBT::LibraryProjectType type = UBT::LIB_FLAGS_NONE;

                if (target["engine"] && target["engine"].as<bool>())
                    type = static_cast<UBT::LibraryProjectType>(type | UBT::LIB_FLAGS_LINK_TO_ENGINE);
                if (target["wrapper"] && target["wrapper"].as<bool>())
                    type = static_cast<UBT::LibraryProjectType>(type | UBT::LIB_FLAGS_LINK_TO_WRAPPER);
                if (target["modded-wrapper"] && target["modded-wrapper"].as<bool>())
                    type = static_cast<UBT::LibraryProjectType>(type | UBT::LIB_FLAGS_LINK_TO_WRAPPER_MODDED);

                data.msvcLinkLibraries.push_back({
                    .target = type,
                    .val = target["target"].as<std::string>()
                });
            }
        }
        if (ll["unix"])
        {
            for (const auto& target : ll["unix"])
            {
                UBT::LibraryProjectType type = UBT::LIB_FLAGS_NONE;

                if (target["engine"] && target["engine"].as<bool>())
                    type = static_cast<UBT::LibraryProjectType>(type | UBT::LIB_FLAGS_LINK_TO_ENGINE);
                if (target["wrapper"] && target["wrapper"].as<bool>())
                    type = static_cast<UBT::LibraryProjectType>(type | UBT::LIB_FLAGS_LINK_TO_WRAPPER);
                if (target["modded-wrapper"] && target["modded-wrapper"].as<bool>())
                    type = static_cast<UBT::LibraryProjectType>(type | UBT::LIB_FLAGS_LINK_TO_WRAPPER_MODDED);

                data.unixLinkLibraries.push_back({
                    .target = type,
                    .val = target["target"].as<std::string>()
                });
            }
        }
    }
}

void UBT::addSourceLibraries(YAML::Node& config, CMakeInfoData& data)
{
    if (config["additional-source-libraries"])
    {
        auto includes = config["additional-source-libraries"];
        if (includes["msvc"])
        {
            auto target = includes["msvc"];
            data.msvcSources.push_back({
                .prjtype = LIB_FLAGS_NONE,
            });

            if (target["engine"])
            {
                data.msvcSources.back().prjtype = static_cast<LibraryProjectType>(data.msvcSources.back().prjtype | LIB_FLAGS_LINK_TO_ENGINE);
                auto engine = target["engine"];
                accumulateHeaderLibraries(engine, data.msvcSources);
            }
            if (target["wrapper"])
            {
                data.msvcSources.back().prjtype = static_cast<LibraryProjectType>(data.msvcSources.back().prjtype | LIB_FLAGS_LINK_TO_WRAPPER);
                auto engine = target["wrapper"];
                accumulateHeaderLibraries(engine, data.msvcSources);
            }
            if (target["modded-wrapper"])
            {
                data.msvcSources.back().prjtype = static_cast<LibraryProjectType>(data.msvcSources.back().prjtype | LIB_FLAGS_LINK_TO_WRAPPER_MODDED);
                auto engine = target["modded-wrapper"];
                accumulateHeaderLibraries(engine, data.msvcSources);
            }
        }
        if (includes["unix"])
        {
            auto target = includes["unix"];
            data.unixSources.push_back({
                .prjtype = LIB_FLAGS_NONE,
            });

            if (target["engine"])
            {
                data.unixSources.back().prjtype = static_cast<LibraryProjectType>(data.unixSources.back().prjtype | LIB_FLAGS_LINK_TO_ENGINE);
                auto engine = target["engine"];
                accumulateHeaderLibraries(engine, data.unixSources);
            }
            if (target["wrapper"])
            {
                data.unixSources.back().prjtype = static_cast<LibraryProjectType>(data.unixSources.back().prjtype | LIB_FLAGS_LINK_TO_WRAPPER);
                auto engine = target["wrapper"];
                accumulateHeaderLibraries(engine, data.unixSources);
            }
            if (target["modded-wrapper"])
            {
                data.unixSources.back().prjtype = static_cast<LibraryProjectType>(data.unixSources.back().prjtype | LIB_FLAGS_LINK_TO_WRAPPER_MODDED);
                auto engine = target["modded-wrapper"];
                accumulateHeaderLibraries(engine, data.unixSources);
            }
        }
    }
}

void UBT::accumulateHeaderLibraries(YAML::Node& engine, std::vector<TargetSource>& headers)
{
    if (engine["glob"])
        for (const auto& a : engine["glob"])
            headers.back().glob.push_back(a.as<std::string>());
    if (engine["individual"])
        for (const auto& a : engine["individual"])
            headers.back().glob.push_back(a.as<std::string>());
}

void UBT::addSubdirectories(YAML::Node& config, CMakeInfoData& data)
{
    if (config["additional-subdirectories"])
    {
        auto includes = config["additional-subdirectories"];
        if (includes["msvc"])
            for (const auto& a : includes["msvc"])
                data.msvcSubdirectories.push_back(a.as<std::string>());
        if (includes["unix"])
            for (const auto& a : includes["unix"])
                data.unixSubdirectories.push_back(a.as<std::string>());
    }
}

void UBT::addFilesToStream(std::ofstream& stream, const std::vector<TargetSource>& src, LibraryProjectType type)
{
    for (const auto& a : src)
    {
        if (a.prjtype & type)
        {
            for (const auto& f : a.glob)
                stream << "\"" << f << "\" ";
            for (const auto& f : a.individual)
                stream << "\"" << f << "\" ";
        }
    }
}
