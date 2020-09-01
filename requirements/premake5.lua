workspace "iw4x"
  configurations "Debug"
  platforms "Win32"

  filter "platforms:x86"
    architecture "x86"
  filter{}

  systemversion "latest"
  cppdialect "C++latest"
  symbols "On"
  editandcontinue "Off"
  flags "NoIncrementalLink"
  buildoptions "/bigobj"
  location "../build"

project "iw4x"
  kind "WindowedApp"
  language "C++"

  files "../iw4x/**"
  includedirs { "../", "../iw4x" }

  pchsource "../iw4x/iw4x.cpp"
  pchheader "iw4x.hpp"
  forceincludes { "iw4x.hpp" }

  linkoptions "/IGNORE:4254 /DYNAMICBASE:NO /SAFESEH:NO /LARGEADDRESSAWARE /LAST:.main"
