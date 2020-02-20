load("@onedal//build/bazel/cc:cc_multidef_lib.bzl", "cc_multidef_lib")
load("@onedal//build/bazel/cc:cc_shared_lib.bzl", "cc_shared_lib")

def _daal_cpu_copts_gcc(cpu):
  march = {
    "sse2":       "nocona",
    "ssse3":      "nocona",
    "sse42":      "corei7",
    "avx":        "sandybridge",
    "avx2":       "haswell",
    "avx512_mic": "haswell",
    "avx512":     "haswell",
  }
  return [ "-march={}".format(march[cpu]) ]

def _daal_cpu_copts_clang(cpu):
  march = {
    "sse2":       "nocona",
    "ssse3":      "core2",
    "sse42":      "nehalem",
    "avx":        "sandybridge",
    "avx2":       "haswell",
    "avx512_mic": "knl",
    "avx512":     "skx",
  }
  return [ "-march={}".format(march[cpu]) ]

def _daal_common_copts_gcc():
  return [
    "-w",
    "-fwrapv",
    "-fno-strict-overflow",
    "-fno-delete-null-pointer-checks",
  ]

def _daal_common_defines_gcc():
  return []

def daal_module(name, copts=[], local_defines=[], **kwargs):
  native.cc_library(
    name = name,
    copts = copts + _daal_common_copts_gcc(),
    local_defines = local_defines + _daal_common_defines_gcc(),
    **kwargs,
  )

def daal_kernel_module(name, hdrs=[], copts=[],
                       local_defines=[], deps=[], **kwargs):
    native.cc_library(
        name = name + "_headers",
        hdrs = hdrs,
    )
    cc_multidef_lib(
        name = name,
        cpus = {
            "sse2":       [ "DAAL_CPU=sse2"       ],
            "ssse3":      [ "DAAL_CPU=ssse3"      ],
            "sse42":      [ "DAAL_CPU=sse42"      ],
            "avx":        [ "DAAL_CPU=avx"        ],
            "avx2":       [ "DAAL_CPU=avx2"       ],
            "avx512_mic": [ "DAAL_CPU=avx512_mic" ],
            "avx512":     [ "DAAL_CPU=avx512"     ],
        },
        fpts = {
            "float":  [ "DAAL_FPTYPE=float"  ],
            "double": [ "DAAL_FPTYPE=double" ],
        },
        copts = copts + _daal_common_copts_gcc(),
        local_defines = local_defines + _daal_common_defines_gcc(),
        deps = deps + [":" + name + "_headers"],
        **kwargs
    )

def daal_shared_lib(name, **kwargs):
    cc_shared_lib(
        name = name,
        **kwargs
    )
