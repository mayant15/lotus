# Code Conventions and Contribution Guidelines

## Guidelines

- Discuss with us before starting to work on an issue.
- Read the documentation first. If you can't find an answer there, that means we need to improve it.

## Compiling from Source

The first thing to ensure is that you're able to compile the engine from source, make changes to it, and then compile again to see them. Refer to the [installation instructions](https://gds.sntiitk.in/lotus/manual/install.html) in our manual.

## Development Flow

- We follow the [GitHub Flow](https://guides.github.com/introduction/flow/), that is, new work needs a new branch, and will be merged via a merge request.
- If you haven't already, create an account at our [GitLab instance](http://git.gds.sntiitk.in/users/sign_in). Remember to register **via email**.
- Create a fork of the Lotus repository. This is where you'll make all your changes.
- Make sure that you have the latest changes, then create a new branch from the latest master.
- Once you're done, send us a merge request.

## Code Organization

- `docs/`: Sphinx documentation and scripts. Built in the CI step and deployed to GitHub pages
- `common/`: Common tools to be used by all subprojects, for example third-party shared libraries
- `editor/`: Editor runtime. Provides a main function for the engine to be used in "editor mode"
- `examples/`: Various example projects for demonstration and testing
- `lotus/`: Core engine libraries. Provide all functionality, linked with the game or editor runtime as a shared library.

## Conventions

### Commit

All commit messages must start with a tag, roughly corresponding to the top-level folder where most of the changes are, like `[editor]`, `[docs]`, `[ci]`, or even systems like `[build]`. These will be used in the future to generate a draft changelog between releases.

Commit headers must be short and in present tense. As a rule of thumb, try and fill the following blank with your header: "When applied, this commit will _________".

### Classes

- If it only stores data, use a struct. If it has more functionality, use a class.
- Always use an `enum class` instead of a C-like `enum`

### Headers

- Use `""` for including files by relative path, and `<>` for including files from compiler include directories
- All header files must start with the `#pragma once` include guard

### Prefixes
- Use `camelCase` or `PascalCase`.
- Raw pointers should be prefixed by `p`.
- Private fields start with a small letter.
- Public fields and member functions start with a capital letter.
- Components should be prefixed by `C`.
    - *Example:* `CTransform`, `CMeshRenderer`
- Interfaces and abstract classes should be prefixed by `I`.
    - *Example:* `ISystem`
- Enums should be prefixed by `E`.
    - *Example:* `EColliderType`
