bin_directory = "../bin/%{cfg.buildcfg}-%{cfg.system}/%{prj.name}"
obj_directory = "../obj/%{cfg.buildcfg}-%{cfg.system}/%{prj.name}"

workspace "pear"
    configurations { "debug", "release" }

include "3rdparty"
include "pear"
include "runtime"
