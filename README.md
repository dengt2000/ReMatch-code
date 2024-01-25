
## More Compact Representation of Automata to Defend against ReDoS Attacks



### Compile

```shell
g++ -O2 *.cpp match.o
```

### Run

```shell
./match.o pattern string
```

### Examples

You can first try a regex and a string to run our algorithm. 

For example:

```shell
./match.o "^(a*)$" "aa"
```



