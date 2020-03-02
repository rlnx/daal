
# Always flags
# -fwrapv
# -fno-strict-overflow
# -fno-delete-null-pointer-checks
# -Wformat
# -Wformat-security
# -D_FORTIFY_SOURCE=2
# -fstack-protector-strong
# -fPIC
# -std=c++11
# -DTBB_SUPPRESS_DEPRECATED_MESSAGES
# -D__TBB_LEGACY_MODE
# -D__TBB_NO_IMPLICIT_LINKAGE
# -DDAAL_NOTHROW_EXCEPTIONS
# -DDAAL_HIDE_DEPRECATED
# -Werror=return-type
# -Werror=uninitialized
# -Werror=unknown-pragmas
# -pedantic
# -Wall
# -Wextra

# Opt flags
# -O2

# Dbg flags
# -g
# -DDEBUG_ASSERT

lnx_icc_flags = [
    "-U_FORTIFY_SOURCE",
    "-fstack-protector",
    "-Wall",
    "-Wthread-safety",
    "-Wself-assign",
    "-Wunused-but-set-parameter",
    "-fcolor-diagnostics",
    "-fno-omit-frame-pointer",
]

lnx_gcc_flags = [

]

all_flags = {
    "lnx": {
        "icc": icc_lnx_flags,
        "gcc": gcc_lnx_flags,
    },
}
