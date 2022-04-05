# Cicely

A collection of small, more or less useless, C programs I've written over the
years.

The name comes from [Myrrhis odorata](https://en.wikipedia.org/wiki/Cicely),
or [Saksankirveli](https://fi.wikipedia.org/wiki/Saksankirveli) in Finnish.


## Pipe repeater - prep

Program that connects multiple instances of the given program into a pipeline.

```sh
prep 1 PROG ARGS
# is same as
PROG ARGS
```

```sh
prep 3 cat -n
# is same as
cat -n | cat -n | cat -n
```

## Yes, or maybe not

Couple takes on the yes utility.

## Other

Simple encoding/decoding programs that I wrote for playing with `prep`:
- mtf - [move-to-front transform](https://en.wikipedia.org/wiki/Move-to-front_transform)
- rle - [run-length encoding](https://en.wikipedia.org/wiki/Run-length_encoding)
