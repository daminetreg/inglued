# Roadmap

## Version 1 - 31. March 2017 : Release basic functionalities
Scope of this version is to support simple use cases for the xxhr and cpp.js needs.

  - [x] Working CMakelists.txt generation
    - `#inglued <>` based compilation
    - `find_package()` system based compilation (hacked in with a mapping for boost for the moment)
    - Working simple test folder and default compilation unit generation 
  - [x] Working INSTALL.md documentation generation
  - [x] Support for boost dependencies and for any `#inglued <>` library. 
  - [x] Documentation for easy installation and for use of cmake feature.
  - [ ] Release for linux
  - [ ] Release for macOS

## Version 2 - Somewhere in End of April : Making it usable for anyone

  - [ ] Make the boost dependency adapter a generic non glued library adapter
    - [ ] Make it github.com connected to check out if a library exists or not. (looking for include path in search or for repo names in organizations).
    - [ ] Transform it to a generic non glued library adapter.
    - [ ] Based on little mapping that users can add themselves in deps/glue.

  - [ ] cmdline to search and add libraries (using the adapter to scan our own sources)

  - [ ] Detect if not at the repository root
  - [ ] Detect if deps/inglued not commited
  - [ ] Improve command line with colors

  - [ ] Release for windows 

