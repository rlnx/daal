
def unique(iterable):
    """Remove duplicates from a list."""
    unique_elements = {element: None for element in iterable}
    return unique_elements.keys()
