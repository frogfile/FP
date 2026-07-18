# Floater

A practice implementation/research of IEEE floats and fixed point numbers.

## Goals

- Look into how IEEE Floats function
- Reimplement them and play around with implementation
- Start implementing fixed point numbers and get tunnel visioned on making them arbitrary sized with non byte aligned point
- Have fun

## Non goals

- Make a usable floats library

## Building

Meant as a library but i use main for testing for now.

```sh
$ git clone https://github.com/frogfile/FP
$ cd FP && make release # alternatively debug / all
```

## On nixos

There's a nixos flake with a dev shell in the root dir

```sh
$ cd FP && nix develop
```

## Contibuting

I don't really care
