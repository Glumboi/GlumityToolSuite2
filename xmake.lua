add_rules("mode.debug", "mode.release")

set_runtimes("MD") -- Use MD for release, MDd for debug

add_requires("minhook")
add_requires("lua")

target("GlumityLib")
    set_kind("static")
    add_files("src/GlumityLib/*.c")
    add_links("user32")
    add_packages("minhook")
    -- Copy the source header and C files to the TCC include folder after building
    after_build(function (target)
        local dest_dir = "src/IL2CPPAPIBridge/default_bridge_env/tcc_include/include/GlumityLib"
        os.mkdir(dest_dir)
        os.cp("src/GlumityLib/*.h", dest_dir)
        os.cp("src/GlumityLib/*.c", dest_dir)
        os.cp("src/GlumityLib/*.asm", dest_dir)
        print("-> GlumityLib source files successfully backed up to TCC environment.")
    end)


-- target("PCBS2OverclockAnyCPUV2")
--     set_targetdir("build/Plugins")
--     set_kind("shared")
--     add_files("src/DemoPlugins/PCBS2OverclockAnyCPUV2/*.cpp")
--     add_links("user32")
--     add_linkdirs("build")
--     add_deps("GlumityLib")
--     add_packages("minhook")
--     add_includedirs("src/GlumityLib")
--     add_includedirs("src/IL2CPPResolver")

    
-- target("PCBS2CheatsV2")
--     set_targetdir("build/Plugins")
--     set_kind("shared")
--     add_files("src/DemoPlugins/PCBS2CheatsV2/*.cpp")
--     add_links("user32")
--     add_linkdirs("build")
--     add_deps("GlumityLib")
--     add_packages("minhook")
--     add_includedirs("src/GlumityLib")
--     add_includedirs("src/IL2CPPResolver")

-- target("DinoScapeOffline")
--     set_targetdir("build/Plugins")
--     set_kind("shared")
--     add_files("src/DemoPlugins/DinoScapeOffline/*.c")
--     add_links("user32")
--     add_linkdirs("build")
--     add_deps("GlumityLib")
--     add_packages("minhook")
--     add_includedirs("src/GlumityLib")
--     add_includedirs("src/IL2CPPResolver")

-- target("HFL-InfiniteMoney")
--     set_targetdir("build/Plugins")
--     set_kind("shared")
--     add_files("src/DemoPlugins/HFL-InfiniteMoney/*.cpp")
--     add_links("user32")
--     add_linkdirs("build")
--     add_deps("GlumityLib")
--     add_packages("minhook")
--     add_includedirs("src/GlumityLib")
--     add_includedirs("src/IL2CPPResolver")

target("IL2CPPAPIBridge")
    set_targetdir("build/Plugins")
    set_kind("shared")
    add_files("src/IL2CPPAPIBridge/*.cpp")
    add_linkdirs("src/IL2CPPAPIBridge/lib")
    add_linkdirs("build")
    add_links("user32")
    add_links("Gdi32")
    add_deps("GlumityLib")
    add_packages("minhook")
    set_languages("c++20")
    add_includedirs("src/GlumityLib")
    add_includedirs("src/IL2CPPResolver")
    add_includedirs("src/IL2CPPAPIBridge/include")
    add_linkdirs("src/IL2CPPAPIBridge/lib") 
    add_links("libtcc") 

target("GlumityV2IL2CPPDumper")
    set_targetdir("build/Plugins")
    set_kind("shared")
    add_files("src/IL2CPPDumper/*.cpp")
    add_links("user32")
    add_linkdirs("build")
    add_deps("GlumityLib")
    add_includedirs("src/GlumityLib")
    add_includedirs("src/IL2CPPResolver")
    add_packages("minhook")
    set_languages("c++20")

target("IL2CPPGUILookup")
    set_targetdir("build/Plugins")
    set_kind("shared")
    add_files("src/IL2CPPGUILookup/*.c")
    add_links("user32")
    add_links("gdi32")
    add_linkdirs("build")
    add_deps("GlumityLib")
    add_includedirs("src/GlumityLib")
    add_includedirs("src/IL2CPPResolver")
    add_packages("minhook")

target("version")
    set_kind("shared")
    add_files("src/versionDllInjector/*.cpp")
    add_links("user32")
    add_linkdirs("build")
    add_includedirs("src/GlumityLib")
    add_packages("minhook")
    add_deps("GlumityLib")
    set_languages("c++20")
-- If you want to known more usage about xmake, please see https://xmake.io
--
-- ## FAQ
--
-- You can enter the project directory firstly before building project.
--
--   $ cd projectdir
--
-- 1. How to build project?
--
--   $ xmake
--
-- 2. How to configure project?
--
--   $ xmake f -p [macosx|linux|iphoneos ..] -a [x86_64|i386|arm64 ..] -m [debug|release]
--
-- 3. Where is the build output directory?
--
--   The default output directory is `./build` and you can configure the output directory.
--
--   $ xmake f -o outputdir
--   $ xmake
--
-- 4. How to run and debug target after building project?
--
--   $ xmake run [targetname]
--   $ xmake run -d [targetname]
--
-- 5. How to install target to the system directory or other output directory?
--
--   $ xmake install
--   $ xmake install -o installdir
--
-- 6. Add some frequently-used compilation flags in xmake.lua
--
-- @code
--    -- add debug and release modes
--    add_rules("mode.debug", "mode.release")
--
--    -- add macro definition
--    add_defines("NDEBUG", "_GNU_SOURCE=1")
--
--    -- set warning all as error
--    set_warnings("all", "error")
--
--    -- set language: c99, c++11
--    set_languages("c99", "c++11")
--
--    -- set optimization: none, faster, fastest, smallest
--    set_optimize("fastest")
--
--    -- add include search directories
--    add_includedirs("/usr/include", "/usr/local/include")
--
--    -- add link libraries and search directories
--    add_links("tbox")
--    add_linkdirs("/usr/local/lib", "/usr/lib")
--
--    -- add system link libraries
--    add_syslinks("z", "pthread")
--
--    -- add compilation and link flags
--    add_cxflags("-stdnolib", "-fno-strict-aliasing")
--    add_ldflags("-L/usr/local/lib", "-lpthread", {force = true})
--
-- @endcode
--

