# Roadmap

## alpha messidor  - version 2 - Somewhere in the End of June : Making it usable for anyone

  - [ ] Allow adding dependencies without needing to commit everything first ( Even the inglued file for the moment need to be committed before) :
    - [ ] Switch to submodules (subtree generates heavy repositories, and we are not really concerned with submodule handling in this case, so submodules are not a bad thing anyway)
    - [ ] Added a command for submodule initialization only at top. Initialize sub-sub-modules (transitive deps) only in a .gitigored deps/transitive folder on `inglued seal`.
  - [ ] for windows and all platform use a master try/catch to print the execution errors
  - [ ] Make the boost dependency adapter a generic non glued library adapter
    - [ ] Make it github.com connected to check out if a library exists or not. (looking for include path in search or for repo names in organizations).
    - [ ] Transform it to a generic non glued library adapter.
    - [ ] Based on little mapping that users can add themselves in deps/glue.

  - [ ] Add possibility to put compiler options (check how to do it compiler agnostively ?)

  - [ ] cmdline to search and add libraries (using the adapter to scan our own sources)

  - [ ] Add cmake generation for application developer (and btw inglue the inglued project)
  - [ ] Write reusable cmake macros instead of generating too-big files.

  - [ ] Detect if not at the repository root
  - [ ] Detect if deps/inglued not commited
  - [ ] Improve command line with colors and parsing ( Use clara ? )

  - [ ] Release for windows 
  - [ ] Create vscode integration of inglued to ease dependencies installation to C++ project

### Other evolutions

  - [x] Create an headeronlyfier which adds all .c/cpp files in an header.
    - [x] Scan sources of non-header-only library
    - [ ] Unity Header: Package all the sources inside an header that can be inclued in the final app
    - [ ] Make this header grow across all steps of transitive inclusion of deps.
    - [ ] Add minification of this header.
  - [ ] Create a modulizer : this big header can be used to generate C++ modules easily.
