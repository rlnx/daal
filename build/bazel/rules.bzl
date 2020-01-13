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
  deps = []
  for fpt in DAAL_FP_TYPES:
    deps.append(name + "_" + fpt)
    _daal_cc_library(
      name = name + "_" + fpt,
      local_defines = local_defines + [ "DAAL_FPTYPE={}".format(fpt) ],
      **kwargs
    )
  return deps

def _daal_cpu_cc_library(name, copts=[], local_defines=[], **kwargs):
  deps = []
  for cpu in DAAL_CPUS:
    deps.append(name + "_" + cpu)
    _daal_cc_library(
      name = name + "_" + cpu,
      copts = copts + _daal_cpu_copts_gcc(cpu),
      local_defines = local_defines + [ "DAAL_CPU={}".format(cpu) ],
      **kwargs
    )
  return deps

def _daal_fpt_cpu_cc_library(name, copts=[], local_defines=[], **kwargs):
  deps = []
  for fpt in DAAL_FP_TYPES:
    for cpu in DAAL_CPUS:
      deps.append(name + "_" + fpt + "_" + cpu)
      _daal_cc_library(
        name = name + "_" + fpt + "_" + cpu,
        copts = copts + _daal_cpu_copts_gcc(cpu),
        local_defines = local_defines + [ "DAAL_FPTYPE={}".format(fpt),
                                          "DAAL_CPU={}".format(cpu) ],
        **kwargs
      )
  return deps

def daal_module(name, srcs=[], deps=[], **kwargs):
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
  local_deps = []
  if len(fpt_files) > 0:
    local_deps += _daal_fpt_cc_library(
      name = name,
      srcs = fpt_files,
      deps = deps,
      **kwargs
    )
  if len(cpu_files) > 0:
    local_deps += _daal_cpu_cc_library(
      name = name,
      srcs = cpu_files,
      deps = deps,
      **kwargs
    )
  if len(fpt_cpu_files) > 0:
    local_deps += _daal_fpt_cpu_cc_library(
      name = name,
      srcs = fpt_cpu_files,
      deps = deps,
      **kwargs
    )
  _daal_cc_library(
    name = name,
    srcs = normal_files,
    deps = deps + local_deps,
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
