# -- Imports -----------------------------------------------------------------
from pathlib import Path
import clang.cindex as cl
import pygit2

# -- Path setup --------------------------------------------------------------
cl.Config.set_library_file('/usr/lib/llvm-14/lib/libclang-14.so')
cl.Config.set_library_path('/usr/lib/llvm-14/lib/libclang-14.so')

# -- Project information -----------------------------------------------------
project = 'StorPool Tasks'
author = 'Iliya Iliev'

# -- Constants ---------------------------------------------------------------

# -- General configuration ---------------------------------------------------
extensions = [
    'sphinx_rtd_theme',
    'myst_parser',
    'sphinx_c_autodoc',
    'sphinx_c_autodoc.viewcode',
    "sphinx_design",
    "sphinx_copybutton"
]

# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']
exclude_patterns = []

# -- API C files -------------------------------------------------------------
c_autodoc_roots = ['../analysis/src/']
c_autodoc_compilation_args = ["-I./../analysis/src/"]

# -- Options for HTML output -------------------------------------------------
html_theme = "sphinx_rtd_theme"

autosectionlabel_prefix_document = True
html_theme_options = {
    'logo_only': False,
    'display_version': True,
    'prev_next_buttons_location': 'bottom',
    'style_external_links': True,
    'vcs_pageview_mode': '',
    'style_nav_header_background': 'rgba(52,49,49,1) 100%;',
    'collapse_navigation': True,
    'sticky_navigation': True,
    'navigation_depth': 2,
    'includehidden': True,
    'titles_only': True,
    'sticky_navigation': True
}

def include_readme_file(app, docname, source):
    """
        This hook reads the contents of the README.md file, replaces the
        link for `git-commit` and inserts the modified contents in the index.md
        file before the first occuarance of  ```{toctree}
    """
    if docname == 'index':
        # Read and modify the contents of README
        readme = Path(app.srcdir) / ".." / "README.md"
        with readme.open("r") as file:
            readme_contents = file.read()

        # Here we change the link for the `git-commit` page
        readme_contents = readme_contents.replace("](./doc/", "](")

        # Find the index of the first occurrence of ```{toctree}
        toctree_index = source[0].find('```{toctree}')
        if toctree_index != -1:
            # Insert the modified README files
            source[0] = source[0][:toctree_index] + readme_contents + source[0][toctree_index:]

def setup(app):
    app.connect('source-read', include_readme_file)

version = pygit2.Repository('.').head.shorthand

html_show_sourcelink = False
html_title = 'StorPool Tasks'
