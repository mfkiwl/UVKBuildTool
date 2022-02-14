#pragma once
#include "Utility.h"

namespace UBT
{
    enum LibraryProjectType
    {
        LIB_FLAGS_NONE = 0,
        LIB_FLAGS_LINK_TO_ENGINE = 1 << 0,
        LIB_FLAGS_LINK_TO_GAME = 1 << 1,
        LIB_FLAGS_LINK_TO_WRAPPER = 1 << 2,
        LIB_FLAGS_LINK_TO_WRAPPER_MODDED = 1 << 3,
        LIB_FLAGS_LINK_TO_ALL = LIB_FLAGS_LINK_TO_ENGINE | LIB_FLAGS_LINK_TO_GAME | LIB_FLAGS_LINK_TO_WRAPPER | LIB_FLAGS_LINK_TO_WRAPPER_MODDED
    };

    struct LibSource
    {
        LibraryProjectType target;
        std::string val;
    };

    struct TargetSource
    {
        LibraryProjectType prjtype;
        std::vector<std::string> glob;
        std::vector<std::string> individual;
    };

    struct CMakeInfoData
    {
        std::vector<std::string> msvcIncludePaths;
        std::vector<std::string> unixIncludePaths;

        std::vector<std::string> msvcSubdirectories;
        std::vector<std::string> unixSubdirectories;

        std::vector<TargetSource> msvcSources;
        std::vector<TargetSource> unixSources;

        std::vector<TargetSource> msvcHeaders;
        std::vector<TargetSource> unixHeaders;

        std::vector<LibSource> msvcLinkLibraries;
        std::vector<LibSource> unixLinkLibraries;
    };

    void addFilesToStream(std::ofstream& stream, const std::vector<TargetSource>& src, LibraryProjectType type);
    void addIncludeDirectories(YAML::Node& config, CMakeInfoData& data);
    void addLinkLibraries(YAML::Node& config, CMakeInfoData& data);
    void addHeaderLibraries(YAML::Node& config, CMakeInfoData& data);
    void addSourceLibraries(YAML::Node& config, CMakeInfoData& data);
    void addSubdirectories(YAML::Node& config, CMakeInfoData& data);

    void generateCmake(const char* name, const CMakeInfoData& data);

    void accumulateHeaderLibraries(YAML::Node& engine, std::vector<TargetSource>& headers);

}