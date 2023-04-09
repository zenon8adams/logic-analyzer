{
  "targets": [
    {
      "target_name":    "logic_analyzer",
	  "include_dirs": [ "<!(node -e \"require('nan')\")", "../../include", "../utility/builder"],
	  "library_dirs": [ "<!(echo $PWD)/", "<!(echo $PWD)/build_deps"],
      "sources":      [  "../utility/builder/json-builder.cpp", "jsLogicAnalyzer.cpp"],
      "cflags_cc":    [ "-std=c++17", "-fexceptions", "-fPIC"],
      "link_settings": {
          "libraries": [
            "-ljs__conn -lanalyzer",
            "-Wl,-rpath=./build_deps"

          ],
          "library_dirs": [
            '<!(pwd)/',
          ],
        },
#       "cflags_cc!":   [ "-fno-rtti"                                           ],
    }
  ]
}
