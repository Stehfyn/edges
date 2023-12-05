-- premake5.lua
include "external/premake/premake_customization/solution_items.lua"

workspace "edges"
    architecture "x64"
    configurations { "Debug", "Release" }
    startproject "test_edges"

    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    solution_items {"premake5.lua", "external.lua"}
    
include "external.lua"