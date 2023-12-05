project "edges"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    targetdir "bin/%{cfg.buildcfg}"
    staticruntime "on"

    files 
    {
        "premake5.lua",
        "**.h",
        "**.hpp",
        "**.cpp"
    }

    includedirs
    {
        ".",
    }

    links
    {

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
        defines { "EDGES_PLATFORM_WINDOWS" }

    filter "configurations:Debug"
        defines { "EDGES_DEBUG" }
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines { "EDGES_RELEASE" }
        runtime "Release"
        optimize "On"
        symbols "On"