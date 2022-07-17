# Doubly Linked List implementation ![Build Status](https://github.com/V13kv/Doubly-Linked-List/workflows/BuildAndTest/badge.svg)
Implementation of [`Doubly Linked List`](https://en.wikipedia.org/wiki/Doubly_linked_list) data structure with beautiful graphical dump using [`Graphviz`](https://graphviz.org/).
Fourth task in MIPT in the first year of education.

## Motivation
The idea is to understand how this data structure is working from the inside, how to beautifully dump it using Graphviz DSL.

## Implementation details
Doubly linked list is implemented to avoid [`memory fragmentation`](https://en.wikipedia.org/wiki/Fragmentation_of_memory).

## Other improvements
1. Universal [`makefile`](https://www.gnu.org/software/make/) was implemented with the minimum amount of actions needed to run any project.

2. Debug library was extended with additional often used functions.

## Setting up
**Clone repository**
```
git clone https://github.com/V13kv/Doubly-Linked-List;
cd Doubly-Linked-List
```

**Compiling**
```
make depends;
make all
```

## Running
```
make run
```

## Cleaning
```
make clean
```
