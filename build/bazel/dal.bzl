load("@onedal//build/bazel/cc:cc_multidef_lib.bzl", "cc_multidef_lib")
load("@onedal//build/bazel/cc:cc_shared_lib.bzl", "cc_shared_lib")

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


