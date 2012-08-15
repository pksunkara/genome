# Genome [![Build Status](https://secure.travis-ci.org/pkumar/genome.png)](http://travis-ci.org/pkumar/genome)

Genome is an esoteric programming language imitating the genome sequence of a living organism

## Installation

```
make install
```

## Usage

Sample hello world program

```
TTC
  ATA GTA GCA
  ATA GTC GTT
  ATA GTC GGA
  AAC
  ATA GTC GGG
  ATA GAC GAA
  ATA GTT GTG
  ACT GAG
  ATA GTG GAC
  ACG GAG
  ATA GTC GTA
  ATA GAC GAT
  ATA GCC
  CTA
TTG CGG
```

```
Hello World!
```

_There are some examples in examples folder_

## Instructions

The virtual machine maintains a stack for you to store things. Each cell in the stack is of size 4 bytes. A genome program starts with `TTC` and ends with `TTC CGG` and it should have the extension `.atcg`. Please check below to know what each instruction means.

* AAA - Duplicate the whole stack
* AAT - Duplicate top n items on the stack
* AAC - Duplicate top item on the stack
* AAG - Duplicate bottom n items on the stack

* ATA - Push number onto stack
* ATT - Pop top n items from stack
* ATC - Pop number from stack (Discarding)
* ATG - Pop bottom n items from stack

* ACA - Clear the whole stack
* ACT - Copy top nth item on the stack
* ACC - Slide n items keeping top item
* ACG - Copy bottom nth item on the stack

* AGA - Reverse the whole stack
* AGT - Reverse the top n items
* AGC - Reverse the top 2 items
* AGG - Reverse the bottom n items

* TAA - Addition 1+2 items and push the result
* TAT - Subtraction 2-1 items and push the result
* TAC - Multiplication 2x1 items and push the result
* TAG - Division 2/1 items and push the result
* TTA - Increment the top item by n (if nothing n=1)
* TTT - Decrement the top item by n (if nothing n=1)

* TTC - Start block
* TTG - Stop block

* TCA - Jump unconditionally to start of the block
* TCT - Jump on top item zero to start of the block
* TCC - Jump on nth item zero to start of the block
* TCG - Jump on bottom item zero to start of the block
* TGA - Jump unconditionally to end of the block
* TGT - Jump on top item zero to end of the block
* TGC - Jump on nth item zero to end of the block
* TGG - Jump on bottom item zero to end of the block

* CAA - Print the whole stack
* CAT - Print top n items of stack
* CAC - Print top item of the stack
* CAG - Print bottom n items of stack
* CTA - Print the whole stack (ASCII)
* CTT - Print top n items of stack (ASCII)
* CTC - Print top item of stack (ASCII)
* CTG - Print bottom n items of stack (ASCII)

* CCA - Read input to n given by top item of stack
* CCT - Read input to top nth of the stack
* CCC - Read input to top of the stack
* CCG - Read input to bottom nth of the stack

* CGA - Move to the top of the stack
* CGT - Move down the stack by n (If nothing, n=1)
* CGC - Move up the stack by n (If nothing, n=1)
* CGG - End program

All the above instructions which contain `n` are/can be followed by a number which is represented by the following

* GAA - Hex 0
* GAT - Hex 1
* GAC - Hex 2
* GAG - Hex 3
* GTA - Hex 4
* GTT - Hex 5
* GTC - Hex 6
* GTG - Hex 7
* GCA - Hex 8
* GCT - Hex 9
* GCC - Hex a
* GCG - Hex b
* GGA - Hex c
* GGT - Hex d
* GGC - Hex e
* GGG - Hex f

All comments are placed in `{}`

So, If I want to push a number 15000 onto stack, I can do the following

```
ATA GAG GCC GCT GCA
{push 0x3a98}
```

## Testing

```
make test
```

## Contribution

Here is a list of [contributors](http://github.com/pkumar/genome/contributors)

__I accept pull requests and guarantee a reply within a day__

## License
MIT/X11

## Bug reports
Report [here](http://github.com/pkumar/genome/issues). __Guaranteed reply within a day__.

## Contact
Pavan Kumar Sunkara (pavan.sss1991@gmail.com)

Follow me on [github](http://github.com/pkumar), [twitter](http://twitter.com/pksunkara)
