load("//build/bazel:cc_rules.bzl", "cc_multidef_library",
                                   "cc_shared_lib")

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
  cc_multidef_library(
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
  return cc_shared_lib(
    name = name,
    **kwargs
  )




def _filter_cpu_cpps(srcs):
  filter_str = '_cpu.cpp'
  filter_len = len(filter_str)
  cpu_srcs, non_cpu_srcs = [], []
  for src in srcs:
    if src[-filter_len:] == filter_str:
      cpu_srcs.append(src)
    else:
      non_cpu_srcs.append(src)
  return cpu_srcs, non_cpu_srcs

def _get_options_for_cpu(cpu):
  return {
    'default': struct(
      flags = ['-DDAL_CPU_ID_=dal::backend::cpu_dispatch_default']
    ),
    'avx': struct(
      flags = ['-mavx', '-DDAL_CPU_ID_=dal::backend::cpu_dispatch_avx']
    ),
    'avx2': struct(
      flags = ['-mavx2', '-DDAL_CPU_ID_=dal::backend::cpu_dispatch_avx2']
    ),
    'avx512': struct(
      flags = ['-mavx512f', '-DDAL_CPU_ID_=dal::backend::cpu_dispatch_avx512']
    ),
  }[cpu]

DAL_DEFAULT_CPUS = [ 'default', 'avx', 'avx2', 'avx512' ]

def dal_module(name, copts=[], **kwargs):
  native.cc_library(
    name = name,
    copts = ['-std=c++17', '-Icpp', '-w'] + copts,
    **kwargs,
  )

def dal_dpcpp_module(name, **kwargs):
  dal_module(name, **kwargs)

def dal_cpu_kernel(name, srcs, cpus=DAL_DEFAULT_CPUS, hdrs=[], deps=[], **kwargs):
  cpu_srcs, non_cpu_srcs = _filter_cpu_cpps(srcs)
  cpu_libs_names = []
  for cpu in cpus:
    cpu_options = _get_options_for_cpu(cpu)
    local_name = name + '_' + cpu
    dal_module(
      name = local_name,
      srcs = cpu_srcs,
      hdrs = hdrs,
      deps = deps,
      copts = cpu_options.flags,
    )
    cpu_libs_names.append(local_name)
  dal_module(
    name = name,
    hdrs = hdrs,
    srcs = non_cpu_srcs,
    deps = cpu_libs_names,
    **kwargs,
  )

def dal_gpu_kernel(**kwargs):
  dal_module(**kwargs)
