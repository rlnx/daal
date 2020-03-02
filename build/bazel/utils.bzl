
def unique(iterable):
    """Remove duplicates from a list."""
    unique_elements = {element: None for element in iterable}
    return unique_elements.keys()

def get_starlark_dict(dictionary):
    entries = [ "\"{}\":\"{}\"".format(k, v) for k, v in dictionary.items() ]
    return ",".join(entries)
