DAAL_FP_TYPES = [
  "float",
  "double",
]

DAAL_CPUS = [
  "sse2",
  # "ssse3",
  # "sse42",
  # "avx",
  # "avx2",
  # "avx512_mic",
  "avx512",
]

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
  return [
    '__int64="long long"',
    '__int32="int"',
  ]

def _daal_cc_library(copts=[], local_defines=[], **kwargs):
  native.cc_library(
    copts = copts + _daal_common_copts_gcc(),
    local_defines = local_defines + _daal_common_defines_gcc(),
    **kwargs
  )

def _daal_fpt_cc_library(name, copts=[], local_defines=[], **kwargs):
  for fpt in DAAL_FP_TYPES:
    _daal_cc_library(
      name = name + "_" + fpt,
      local_defines = local_defines + [ "DAAL_FPTYPE={}".format(fpt) ],
      **kwargs
    )

def _daal_cpu_cc_library(name, copts=[], local_defines=[], **kwargs):
  for cpu in DAAL_CPUS:
    _daal_cc_library(
      name = name + "_" + cpu,
      copts = copts + _daal_cpu_copts_gcc(cpu),
      local_defines = local_defines + [ "DAAL_CPU={}".format(cpu) ],
      **kwargs
    )

def _daal_fpt_cpu_cc_library(name, copts=[], local_defines=[], **kwargs):
  for fpt in DAAL_FP_TYPES:
    for cpu in DAAL_CPUS:
      _daal_cc_library(
        name = name + "_" + fpt + "_" + cpu,
        copts = copts + _daal_cpu_copts_gcc(cpu),
        local_defines = local_defines + [ "DAAL_FPTYPE={}".format(fpt),
                                          "DAAL_CPU={}".format(cpu) ],
        **kwargs
      )

def daal_module(srcs=[], **kwargs):
  if not len(srcs):
    _daal_cc_library(**kwargs)
  fpt_files = []
  cpu_files = []
  normal_files = []
  fpt_cpu_files = []
  for src in srcs:
    is_cpu_file = "_cpu" in src
    is_fpt_file = "_fpt" in src
    if is_fpt_file and is_cpu_file:
      fpt_cpu_files.append(src)
    elif is_cpu_file:
      cpu_files.append(src)
    elif is_fpt_file:
      fpt_files.append(src)
    else:
      normal_files.append(src)
  if len(normal_files) > 0:
    _daal_cc_library(srcs=normal_files, **kwargs)
  if len(fpt_files) > 0:
    _daal_fpt_cc_library(srcs=fpt_files, **kwargs)
  if len(cpu_files) > 0:
    _daal_cpu_cc_library(srcs=cpu_files, **kwargs)
  if len(fpt_cpu_files) > 0:
    _daal_fpt_cpu_cc_library(srcs=fpt_cpu_files, **kwargs)
