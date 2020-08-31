# Definitions

This repository contains common definitions to be used in LaTeX documents. Just use this as a submodule within your repository and include the .tex-files you want to use:

```
git submodule add git@collaborating.tuhh.de:e-4/utilities/definitions.git definitions
```

## Minimal working example
```
\documentclass[a4paper]{article}
\input{definitions/packages.tex}
\input{definitions/commands.tex}
\input{definitions/acronyms.tex}

\addbibresource{library.bib}

\begin{document}
	Hello World!
	
	\printbibliography
\end{document}
```

## Acronyms

These are to be used with the LaTeX acronym package.
Declare the usage of said package via e.g. `\usepackage[printonlyused]{acronym}` beforehand.
If you don't want to print the list of acronyms, as e.g. in a conference paper, you can also use `\usepackage[nolist]{acronym}`.
Somewhere after the `\begin{document}` command, whereever you want to print the list of acronyms, then include the acronyms.tex file via `\input{path/to/acronyms}`.
Usually this could be placed within two `\cleardoublepage` commands. Example if my acronyms submodule is located in the `definitions` folder of my project root:

`\begin{document}`   
`...`   
`\cleardoublepage`   
`\input{definitions/acronyms}`   
`\cleardoublepage`   

### Structure of entries

`\acro{1}[2]{3}`

1.  Acronym to be used within LaTeX code with e.g. `\ac{}`. Keep it lower-case and alphanumeric (only a-z and 0-9) so that nobody has to remember the exact spelling (e.g. where a hyphen is or the correct lower and upper case composition)
2.  Short version to appear in the final text
3.  Long version to appear in the fiinal test

### Sorting and removing duplicates within acronym file

You can pipe everything within the `acronym` block through `sort` and `uniq`.
