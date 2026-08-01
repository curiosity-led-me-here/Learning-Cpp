## Implementing various libraries in an attempt to learn C++.

The purpose of this repository is to understand how programs are structured internally rather than relying entirely on prebuilt libraries and black-box abstractions.
This is an active learning repository. Implementations may be incomplete, experimental, or rewritten as my understanding improves.

## Previous Implementation: Hamming Code

The objective was to build out an single bit error correction code. The way it works is it assigns parity bits to the bit-grid which switch on when certain regions of the data grid have odd 1-bits. The data along with these set parity bits are then transmitted to the receiver. In the event of corruption of one of the bits, the parity bits would supply the require information to pinpoint the location of corruption. 
The problem statement was earlier backups took up almost >50% by storing entire data in copies. However this compression algorithm uses about log_2(n) + 1 bits to store compressed backups.
While in reality, corruption is barely restricted to single bit, and is usually in swarms which this code fails to correct, this was experiemental and just to learn C++ semantics.

## Current Implementation: Tensor Library

The objective is to build out a library that performs various transformations on multi-dimensional arrays.
The purpose is to understand and use my own tensor library for future ML projects rather than relying on existing libs.

The implementation stores tensor values in a flat one-dimensional vector and uses dimension mapping using stride metadata to interpret that as a arbitrary rank tensor.
