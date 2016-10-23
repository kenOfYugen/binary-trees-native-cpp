{
  "targets": [
    {
      "includes": [
        "auto.gypi"
      ],
      "sources": [
        "src/binarytrees.cc"
      ],
      "cflags_cc": [
        "-pipe",
        "-O3",
        "-fomit-frame-pointer",
        "-march=native",
        "-std=c++14",
        "-fopenmp"
      ],
      "link_settings":{
        "ldflags": [
          "-L/usr/lib -lboost_system -lgomp"
        ]
      }
    }
  ],
  "includes": [
    "auto-top.gypi"
  ]
}
