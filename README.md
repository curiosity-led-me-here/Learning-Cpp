## Implementing various libraries in an attempt to learn C++.

The purpose of this repository is to understand how programs are structured internally rather than relying entirely on prebuilt libraries and black-box abstractions.
This is an active learning repository. Implementations may be incomplete, experimental, or rewritten as my understanding improves.

## Current Implementation: Tensor Library

The objective is to build out a library that performs various transformations on multi-dimensional arrays.
The purpose is to understand and use my own tensor library for future ML projects rather than relying on existing libs.

The implementation stores tensor values in a flat one-dimensional vector and uses dimension mapping using stride metadata to interpret that as a arbitrary rank tensor.
