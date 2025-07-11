workspace "SvarnEngine"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}


outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "SvarnEngine"
	location "SvarnEngine"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/vendor/spdlog/include"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"
		toolset "v143"


		defines
		{
			"SV_PLATFORM_WINDOWS",
			"SV_BUILD_DLL"
		}

		postbuildcommands {
			'{COPY} "%{cfg.buildtarget.abspath}" "../bin/' .. outputdir .. '/Sandbox/"'
		}

	filter { "system:windows", "action:vs*" }
		postbuildcommands {
			'{COPY} "%{cfg.buildtarget.abspath}" "../bin/' .. outputdir .. '/Sandbox/"'
		}

	filter "system:linux"
		cppdialect "C++17"
		staticruntime "Off"
		pic "On"

		defines
		{
			"SV_PLATFORM_LINUX",
			"SV_BUILD_DLL"
		}

		postbuildcommands {
			'{COPYFILE} "%{cfg.buildtarget.abspath}" "../bin/' .. outputdir .. '/Sandbox/"'
		}

	filter "configurations:Debug"
		defines "SV_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "SV_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "SV_DIST"
		optimize "On"

	filter "action:vs*"
		buildoptions { "/utf-8" }

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"SvarnEngine/vendor/spdlog/include",
		"SvarnEngine/src"
	}

	links
	{
		"SvarnEngine"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"SV_PLATFORM_WINDOWS"
		}

	filter "system:linux"
		cppdialect "C++17"
		staticruntime "Off"
		pic "On"

		defines {
			"SV_PLATFORM_LINUX"
		}

		postbuildcommands {
			'mkdir -p ../bin/' .. outputdir .. '/Sandbox/'
		}

	filter "configurations:Debug"
		defines "HZ_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "HZ_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "HZ_DIST"
		optimize "On"

	filter "action:vs*"
		buildoptions { "/utf-8" }