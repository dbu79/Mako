# Mako

Mako is an interpreted language written in C++. 

## Syntax

```
fn helloWorld(){
    print "Hello, World!";
}

fn fib(n) {
        if (n<=1) return n;
        return fib(n-2) + fib(n-1);
    }

    for (var i=0; i<20; i++){
            print(fib(i));
    }
```

## How It Works

```
source -> [lexer] -> flat token stream -> [parser] -> syntax tree -> [interpreter] -> output
```

## To Do 
- Static methods
- Getter methods
- Arrays and other data structures
