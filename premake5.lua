workspace "Black_Cat_Workspace"
    architecture "x86"
    startproject "Black_Cat_Project"

    configurations{
        "Debug",
        "Release"
    }

    

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "Black_Cat_Project"

newaction{
    trigger = "clean",
    description = "Remove all binaries, intermediate binaries and vs files",
    execute = function()
        print("Removing binaries")
        os.rmdir("./bin")
        print("Removing int binaries")
        os.rmdir("./obj")
        print("Removing project files")
        os.rmdir("./.vs")
        os.remove("**.sln")
        os.remove("**.vcxproj")
        os.remove("**.vcxproj.filters")
        os.remove("**.vcxproj.users")
        os.remove("**.vcxproj.user")
        print("Done!")
    end
}