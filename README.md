# To build:

## Windows
```
mingw32-make
```
## Linux
```
make
```

# Examples

Basic mathematical operations:

```
> 3 + 3
> 3 - 3
> 3 * 3
> 3 / 3
> 3 ^ 3
> (3 + 3) * 2
```
 
Concatenate strings:

```
> "Hello, " + "World!"
> 'Python ' + 'is ' + 'slow'
```

Math functions:

```
> sin(90) + 32
> atan2(90, 180)
```

Declare variables, this works both for declaring new variables and reassigning existing variables:

```
> let three = 3
> let hi = 'hi'
> let hello = "hello"
> let hello = 77
```

Using variables:

```
> $three + 3
> $hi + $hello
```

Boolean expressions:

```
> $hi == $hello || $three == 3
> true && true
> 3 < 89
> 90 > $three
> 1 <= 1
> (2 >= 90 && false) || (33 < 890 && true != false)
```

Importing and Exporting variables (experimental):

```
> ./PrattParsing
> let hi = 'hi'
> export($hi, 'hi.txt')
> true
> exit
```

```
> ./PrattParsing
> let hi2 = import('hi', 'hi.txt')
> hi
```
