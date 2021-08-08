{
  "targets": [
    {
      "target_name":    "trial",
	  "include_dirs": [ "./node_modules/node-addon-api", "../include"         ],
      "sources":      [ "trial.cpp"                                           ],
      "cflags_cc":    [ "-std=c++17", "-fpermissive" , "-fexceptions", "-fPIC"],
      "libraries":    [
        "-lanalyzer",
        "-L<!(echo $PWD)/../build/Release",
        "-Wl,-rpath=<!(echo $PWD)/../build/Release"
      ],
      "cflags_cc!":   [ "-fno-rtti"                                           ],
      "defines":      [ "NAPI_CPP_EXCEPTIONS",                                ],
    }
  ]
}
