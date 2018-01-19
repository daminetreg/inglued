# Roadmap

## alpha floréal - version 2 - Somewhere in End of April : Making it usable for anyone

  - [ ] for windows and all platform use a master try/catch to print the execution errors
  - [ ] Make the boost dependency adapter a generic non glued library adapter
    - [ ] Make it github.com connected to check out if a library exists or not. (looking for include path in search or for repo names in organizations).
    - [ ] Transform it to a generic non glued library adapter.
    - [ ] Based on little mapping that users can add themselves in deps/glue.

  - [ ] Add possibility to put compiler options (check how to do it compiler agnostively ?)

  - [ ] cmdline to search and add libraries (using the adapter to scan our own sources)

  - [ ] Add cmake generation for application developer (inglue the inglued project)

  - [ ] Detect if not at the repository root
  - [ ] Detect if deps/inglued not commited
  - [ ] Improve command line with colors

  - [ ] Release for windows 

### Other evolutions

  - [ ] Create an headeronlyfier which adds all .c/cpp files in an header.
    - [ ] Scan sources of non-header-only library
    - [ ] Package all the sources inside an header that can be inclued in the final app
    - [ ] Make this header grow across all steps of transitive inclusion of deps.
    - [ ] Add minification of this header.
  - [ ] Create a modulizer : this big header can be used to generate C++ modules easily.
