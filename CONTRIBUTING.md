# Code Conventions and Contribution Guidelines

## Guidelines

- Discuss with us before starting to work on an issue.
- Read the documentation first. If you can't find an answer there, that means we need to improve it.

## Code Organization

- `docs/`: Sphinx documentation and scripts. Built in the CI step and deployed to GitHub pages
- `common/`: Common tools to be used by all subprojects, for example third-party shared libraries
- `editor/`: Editor runtime. Provides a main function for the engine to be used in "editor mode"
- `examples/`: Various example projects for demonstration and testing
- `lotus/`: Core engine libraries. Provide all functionality, linked with the game or editor runtime as a shared library.

## Conventions

- If it only stores data, use a struct. If it has more functionality, use a class.

### Headers

- Use `""` for including project files, and `<>` for including external dependencies
- All header files must start with the `#pragma once` include guard

### Prefixes
- Use `camelCase` or `PascalCase`.
- Unique pointer typedefs should be prefixed by `URef`.
    - *Example:* `typedef std::unique_ptr<Sample> URefSample`
- Shared pointer typedefs should be prefixed by `SRef`.
    - *Example:* `typedef std::shared_ptr<Sample> SRefSample`
- Raw pointers should be prefixed by `p`.
- Private fields start with a small letter.
- Public fields and member functions start with a capital letter.
- Actors should be prefixed by `A`.
    - *Example:* `ALightSource`, `ABox`
- Components should be prefixed by `C`.
    - *Example:* `CTransform`, `CMesh`
- Interfaces and abstract classes should be prefixed by `I`.
    - *Example:* `IComponent`, `IRenderer`
