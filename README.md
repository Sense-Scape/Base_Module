# Base_Module

## Summary

This repository aims to implement the foundations of a generic modular multithreaded processing chain. It attempts to allow one to 
encapsulate processing functionality into each module. Modules are connected to one another forming a processing pipeline. Each module 
will operate on different chunk types as described by [this](https://github.com/Sense-Scape/Chunk_Types) repository.

## Getting Started

One should first pull this repository and update the submodules as it does required the DocTest header to build. 
The Doctest library has been branched into this organisation. **Note** that this is intended to be used a library.

### Windows

- One should have [Visual Studio 2022 downloaded](https://visualstudio.microsoft.com/vs/)
- Ensure this repository is pulled
- Open the solution file denoted by the ```.sln```
- Run the build command
- One should get an executable test file as well as a `.lib` file

### Linux

There is currently no support for linux but there is an intention to add it when required.

## Repository Structure

``` mermaid

flowchart TD

    subgraph BaseModule

        subgraph Components_BM

            subgraph BaseChunk

                subgraph Components_BC
                
                    subgraph DocTest

                    end

                    subgraph Nlohmann_JSON

                    end

                end
                
            end

            

            subgraph Plog
            end


            subgraph doctest
            end

        end

    end
```

