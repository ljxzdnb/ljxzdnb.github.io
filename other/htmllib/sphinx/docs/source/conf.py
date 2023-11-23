# Configuration file for the Sphinx documentation builder.

# -- Project information

project = 'C data struct'
copyright = '2022, ljx'
author = 'ljx'

release = '0.1'
version = '0.1.0'

# -- General configuration

extensions = [
    'myst_parser',
    'sphinxcontrib.mermaid',
    'sphinx.ext.duration',
    'sphinx.ext.doctest',
    'sphinx.ext.autodoc',
    'sphinx.ext.autosummary',
    'sphinx.ext.intersphinx',
    ]
myst_enable_extensions = [
    'amsmath',
    #'dollarmath',
]
intersphinx_mapping = {
    'python': ('https://docs.python.org/3/', None),
    'sphinx': ('https://www.sphinx-doc.org/en/master/', None),
}
intersphinx_disabled_domains = ['std']

templates_path = ['_templates']

# -- Options for HTML output

#html_theme = 'sphinx_rtd_theme'

#html_theme_options = {
#  "page_sidebar_items": ["page-toc", "edit-this-page"],
#  "navbar_start": ["navbar-logo"],
#  "navbar_center": ["navbar-nav"],
#  "navbar_end": ["navbar-icon-links"]
#}
#html_theme = 'furo'
#html_theme = 'sphinx_material'
#html_logo = 'fotile.PNG' 
# Material theme options (see theme.conf for more information)
html_theme_options = {

    # Set the name of the project to appear in the navigation.
    'nav_title': 'C data struct info',

    # Set you GA account ID to enable tracking
    'google_analytics_account': 'UA-XXXXX',

    # Specify a base_url used to generate sitemap.xml. If not
    # specified, then no sitemap will be built.
    'base_url': 'https://project.github.io/project',

    # Set the color and the accent color
    'color_primary': 'white',
    'color_accent': 'light-white',

    # Set the repo location to get a badge with stats
    'repo_url': 'https://github.com/project/project/',
    'repo_name': 'c struct data',

    # Visible levels of the global TOC; -1 means unlimited
    'globaltoc_depth': 1,
    # If False, expand all TOC entries
    'globaltoc_collapse': True,
    # If True, show hidden TOC entries
    'globaltoc_includehidden': True,
}
html_sidebars = {
    "**": ["logo-text.html", "globaltoc.html", "localtoc.html", "searchbox.html"]
}


# -- Options for EPUB output
epub_show_urls = 'footnote'
