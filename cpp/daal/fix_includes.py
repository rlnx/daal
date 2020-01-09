import os
import re
import glob
from collections import defaultdict

_include_re = re.compile(r' *#include +"(.+)"')

def find_all(names, path='.'):
    result = {}
    for root, dirs, files in os.walk(path):
        for name in names:
            name_added = False
            for file in files:
                full_path = os.path.join(root, file)
                if name in full_path:
                    result[name] = full_path
                    name_added = True

    for name in names:
        if not name in result:
            print(f'WARNING: Cannot find path for "{name}"')
    return result

def normalize_include(included_filename: str):
    fragments = [ './', '../' ]
    result = included_filename
    need_replace_more = True
    while need_replace_more:
        need_replace_more = False
        for frag in fragments:
            if result.startswith(frag):
                result = result.replace(frag, '', 1)
                need_replace_more = True
    return result

def process_file(filename):
    includes = []
    with open(filename) as f:
        for line in f:
            match = _include_re.match(line)
            if match:
                include = normalize_include(match.group(1))
                includes.append(include)
    return includes

def main():
    h_files = glob.glob(f'./**/*.h', recursive=True)
    cpp_files = glob.glob(f'./**/*.cpp', recursive=True)
    i_files = glob.glob(f'./**/*.i', recursive=True)
    files = h_files + cpp_files + i_files
    filename_to_include_map = {}
    all_includes_flat = []
    for file in files:
        includes = process_file(file)
        filename_to_include_map[file] = includes
        all_includes_flat += includes
    all_includes_set = set(all_includes_flat)
    include_to_file_mapping = find_all(all_includes_set)

    for filename, includes in filename_to_include_map.items():
        with open(filename, 'r') as input_file:
            output_content = ''
            for line in input_file:
                replacement_happend = False
                if '#include' in line:
                    for include in includes:
                        if include in line and include in include_to_file_mapping:
                            full_include = include_to_file_mapping[include]
                            full_include_fixed = full_include.replace('./include/', '').replace('./', '')
                            output_content += f'#include "{full_include_fixed}"\n'
                            replacement_happend = True
                            break
                if not replacement_happend:
                    output_content += line
        with open(filename, 'w') as output_file:
            output_file.write(output_content)

if __name__ == "__main__":
    main()
