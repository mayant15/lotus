# Code Conventions and Contribution Guidelines

## Guidelines

> When in doubt, copy Unreal Engine

## Conventions

- If it only stores data, use a struct. If it has more functionality, use a class.

### Headers

- Use `""` for including project files, and `<>` for including external dependencies
- All header files must start with the `#pragma once` include guard

### Prefixes
- Unique pointer typedefs should be prefixed by `URef`.
    - *Example:* `typedef std::unique_ptr<Sample> URefSample`
- Shared pointer typedefs should be prefixed by `SRef`.
    - *Example:* `typedef std::shared_ptr<Sample> SRefSample`
- Actors should be prefixed by `A`.
    - *Example:* `ALightSource`, `ABox`
- Components should be prefixed by `C`.
    - *Example:* `CTransform`, `CMesh`
- Interfaces and abstract classes should be prefixed by `I`.
    - *Example:* `IComponent`, `IRenderer`
