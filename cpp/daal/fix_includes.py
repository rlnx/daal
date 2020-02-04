import os
import re
import glob
import argparse

def superglob(patterns, extensions=[''], root=''):
    for pattern in patterns:
        for extension in extensions:
            search_pattern = os.path.join(root, pattern + extension)
            for path in glob.iglob(search_pattern, recursive=True):
                yield path

def normalize_include(include: str):
    normalized_include = os.path.normpath(include)
    return normalized_include.replace('../', '')

def normalize_include_full_path(config, include_full_path: str):
    normalized_path = os.path.normpath(include_full_path)
    normalized_path = os.path.relpath(normalized_path, config.root)
    normalized_path = normalized_path.replace('include/', '')
    return normalized_path

_include_re = re.compile(r' *#include +"(.+)"')
def try_extract_include(line):
    match = _include_re.match(line)
    if match:
        return match.group(1)

_includes_cache = {}
_includes_glob_cache = None
def find_full_include_path(config, include):
    global _includes_cache
    global _includes_glob_cache
    variants = []
    normalized_include = normalize_include(include)
    if normalize_include in _includes_cache:
        return _includes_cache[normalize_include]
    if _includes_glob_cache is None:
        glob_iter = superglob(config.directories_pattern,
                              config.include_extensions,
                              config.root)
        _includes_glob_cache = list(glob_iter)
    for filename in _includes_glob_cache:
        if ('./' + filename).endswith('/' + normalized_include):
            normalized_full_path = normalize_include_full_path(config, filename)
            variants.append(normalized_full_path)
    if len(variants) == 1:
        _includes_cache[normalized_include] = variants[0]
    return variants

def replace_includes_in_file(config, filename):
    output_file_lines = []
    any_replacement_happend = False
    with open(filename, 'r') as input_file:
        for line in input_file:
            is_replacement_happend = False
            include = try_extract_include(line)
            if include:
                full_include_paths = find_full_include_path(config, include)
                if len(full_include_paths) == 1:
                    is_replacement_happend = True
                    any_replacement_happend = True
                    include_replacement = f'#include "{full_include_paths[0]}"\n'
                    output_file_lines.append(include_replacement)
                elif len(full_include_paths) == 0:
                    print(f'WARNING: Cannot find full path to "{include}" in "{filename}"')
                elif len(full_include_paths) > 0:
                    print(f'WARNING: Include "{include}" matches several full paths in "{filename}":')
                    for full_include_path in full_include_paths:
                        print(f'   {full_include_path}')
            if not is_replacement_happend:
                output_file_lines.append(line)
    if (not config.test) and any_replacement_happend:
        with open(filename, 'w') as output_file:
            output_file.writelines(output_file_lines)

def process_files(config):
    for filename in superglob(config.directories_pattern,
                              config.include_users_extensions,
                              config.root):
        if config.progress:
            print(filename)
        replace_includes_in_file(config, filename)

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('--test', action='store_true', default=False)
    parser.add_argument('--progress', action='store_true', default=False)
    parser.add_argument('--root', type=str, default='')
    config = parser.parse_args()
    config.directories_pattern = [
        'include/algorithms/**/*',
        'include/data_management/**/*',
        'include/oneapi/**/*',
        'include/services/**/*',
        'src/algorithms/**/*',
        'src/externals/*',
        'src/services/**/*',
    ]
    config.include_extensions = [ '.h', '.i', '.cl' ]
    config.include_users_extensions = [ '.h', '.i', '.cpp' ]
    process_files(config)
