project "test_edges"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    targetdir "bin/%{cfg.buildcfg}"
    staticruntime "on"

    files 
    {
        "premake5.lua",
        "**.cpp",
    }

    includedirs
    {
        ".",
        "../%{IncludeDir.edges}"
    }

    links
    {
        "edges",
    }

    libdirs
    {

    }

    defines
    {

    }

    targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

    filter "system:windows"
        systemversion "latest"
        defines { "EDGES_TEST_PLATFORM_WINDOWS" }

    filter "configurations:Debug"
        defines { "EDGES_TEST_DEBUG" }
        runtime "Debug"
        optimize "Off"
        symbols "On"

    filter "configurations:Release"
        defines { "EDGES_TEST_RELEASE" }
        runtime "Release"
        optimize "On"
        symbols "Off"