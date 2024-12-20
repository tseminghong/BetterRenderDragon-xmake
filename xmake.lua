add_rules("mode.debug", "mode.release")

add_repositories("liteldev-repo https://github.com/LiteLDev/xmake-repo.git")

add_requires("detours v4.0.1-xmake.1")
add_requires("nlohmann_json")
add_requires("gsl v4.0.0")
add_requires("imgui v1.91.0-docking", {configs = {dx11 = true, dx12 = true}})


target("BetterRenderDragon")
    set_kind("shared")
    set_strip("all")
    set_languages("c++20")
    set_exceptions("none")
    add_headerfiles("src/(**.h)")
    add_includedirs("./src")
    add_includedirs("./include")
    add_defines("UNICODE")
    add_cxflags("/utf-8", "/EHa")
    add_files("src/**.cpp")
    add_packages("detours", "gsl", "imgui", "nlohmann_json")
    add_links("runtimeobject","dxgi")
    if is_mode("release") then
        set_symbols("debug")
    end