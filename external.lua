-- external.lua
IncludeDir = {}
IncludeDir["edges"]  = "../include"
LibraryDir = {}

group "external"
group ""

group "tests"
   include "tests/test_edges"
group ""

group "core"
   include "include/"
group ""