set_project("adventofcode")
set_version("2023")

set_policy("build.warning", true)
set_warnings("allextra")
set_languages("cxx17")
set_encodings("utf-8")

target("01a")
    set_kind("binary")
    add_files("01a.cc")

target("01b")
    set_kind("binary")
    add_files("01b.cc")

target("02a")
    set_kind("binary")
    add_files("02a.cc")

target("02b")
    set_kind("binary")
    add_files("02b.cc")

target("03a")
    set_kind("binary")
    add_files("03a.cc")

target("03b")
    set_kind("binary")
    add_files("03b.cc")

target("04a")
    set_kind("binary")
    add_files("04a.cc")

target("04b")
    set_kind("binary")
    add_files("04b.cc")