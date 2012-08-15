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

### Stack manipulation

* __AAA__ - Duplicate the whole stack
* __AAT__ - Duplicate top n items on the stack
* __AAC__ - Duplicate top item on the stack
* __AAG__ - Duplicate bottom n items on the stack
* __ATA__ - Push number onto stack
* __ATT__ - Pop top n items from stack
* __ATC__ - Pop number from stack (Discarding)
* __ATG__ - Pop bottom n items from stack
* __ACA__ - Clear the whole stack
* __ACT__ - Copy top nth item on the stack
* __ACC__ - Slide n items keeping top item
* __ACG__ - Copy bottom nth item on the stack
* __AGA__ - Reverse the whole stack
* __AGT__ - Reverse the top n items
* __AGC__ - Reverse the top 2 items
* __AGG__ - Reverse the bottom n items

### Arithematic

* __TAA__ - Addition 1+2 items and push the result
* __TAT__ - Subtraction 2-1 items and push the result
* __TAC__ - Multiplication 2x1 items and push the result
* __TAG__ - Division 2/1 items and push the result
* __TTA__ - Increment the top item by n (if nothing n=1)
* __TTT__ - Decrement the top item by n (if nothing n=1)

### Blocks and Jumps

* __TTC__ - Start block
* __TTG__ - Stop block
* __TCA__ - Jump unconditionally to start of the block
* __TCT__ - Jump on top item zero to start of the block
* __TCC__ - Jump on nth item zero to start of the block
* __TCG__ - Jump on bottom item zero to start of the block
* __TGA__ - Jump unconditionally to end of the block
* __TGT__ - Jump on top item zero to end of the block
* __TGC__ - Jump on nth item zero to end of the block
* __TGG__ - Jump on bottom item zero to end of the block

### Printing

* __CAA__ - Print the whole stack
* __CAT__ - Print top n items of stack
* __CAC__ - Print top item of the stack
* __CAG__ - Print bottom n items of stack
* __CTA__ - Print the whole stack (ASCII)
* __CTT__ - Print top n items of stack (ASCII)
* __CTC__ - Print top item of stack (ASCII)
* __CTG__ - Print bottom n items of stack (ASCII)

### Scanning

* __CCA__ - Read input to n given by top item of stack
* __CCT__ - Read input to top nth of the stack
* __CCC__ - Read input to top of the stack
* __CCG__ - Read input to bottom nth of the stack

### Miscellaneous

* __CGA__ - Move to the top of the stack
* __CGT__ - Move down the stack by n (If nothing, n=1)
* __CGC__ - Move up the stack by n (If nothing, n=1)
* __CGG__ - End program

### Numbers

All the above instructions which contain `n` are/can be followed by a number which is represented by the following

* __GAA__ - Hex 0
* __GAT__ - Hex 1
* __GAC__ - Hex 2
* __GAG__ - Hex 3
* __GTA__ - Hex 4
* __GTT__ - Hex 5
* __GTC__ - Hex 6
* __GTG__ - Hex 7
* __GCA__ - Hex 8
* __GCT__ - Hex 9
* __GCC__ - Hex a
* __GCG__ - Hex b
* __GGA__ - Hex c
* __GGT__ - Hex d
* __GGC__ - Hex e
* __GGG__ - Hex f

### Comments

All comments are placed in `{}`

### Example

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
