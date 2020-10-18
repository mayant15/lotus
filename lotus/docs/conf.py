import subprocess
import os

# -- Project information -----------------------------------------------------

project = 'Lotus'
copyright = '2020, Studio Centauri'
author = 'Mayant Mukul'

# The full version, including alpha/beta/rc tags
release = '0.0.1'

# -- General configuration ---------------------------------------------------

# Tell sphinx what the primary language being documented is.
primary_domain = 'cpp'

# Tell sphinx what the pygments highlight language should be.
highlight_language = 'cpp'

extensions = [
    "breathe",
    "exhale"
]

# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']

# -- Options for HTML output -------------------------------------------------

html_theme = 'sphinx_rtd_theme'

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ['_static']
html_logo = '_static/images/logo.png'
html_css_files = [
    'css/navbar.css'
]

html_theme_options = {
    'logo_only': True
}


# -- Read The Docs ----------------------------------------------------------

def configure_doxyfile(input_dir, output_dir, exclude_dir):
    with open('Doxyfile.in', 'r') as file:
        filedata = file.read()

    filedata = filedata.replace('@DOXYGEN_INPUT_DIR@', input_dir)
    filedata = filedata.replace('@DOXYGEN_OUTPUT_DIR@', output_dir)
    filedata = filedata.replace('@DOXYGEN_EXCLUDE_DIR@', exclude_dir)

    with open('Doxyfile', 'w') as file:
        file.write(filedata)


# Check if we're running on Read the Docs' servers
read_the_docs_build = os.environ.get('READTHEDOCS', None) == 'True'

breathe_projects = {}

if read_the_docs_build:
    input_dir = '../include/lotus'
    output_dir = 'doxygen'
    exclude_dir = input_dir + '/internal'
    configure_doxyfile(input_dir, output_dir, exclude_dir)
    subprocess.call('doxygen', shell=True)
    breathe_projects['lotus'] = output_dir + '/xml'

# Breathe Configuration
breathe_default_project = "lotus"

# Exhale configuration
exhale_args = {
    # Required arguments
    "containmentFolder": "api",
    "rootFileName": "lotus.rst",
    "rootFileTitle": "API Reference",
    "doxygenStripFromPath": "../",

    # Suggested optional arguments
    "createTreeView": True,
    "exhaleExecutesDoxygen": False,
}
