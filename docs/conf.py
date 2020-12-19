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

extensions = ["breathe", "sphinx.ext.autosectionlabel"]

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

# Breathe Configuration
breathe_projects = {}
breathe_default_project = "lotus"
