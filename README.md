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

class Person {
    fn info() {
        var name = "Daniel";
        print "Your age is " + this.age + " and your name is " + name;
    }
}
var person = Person();
person.age = "19";
person.info();

class Person2 {
    fn init(name, age) {
        this.name = name;
        this.age = age;
    }
    fn info() {
        print "Your name is " + this.name + " and your age is " + this.age;
    }
}

var person2 = Person2("Matthew", 21);
person2.info();
```

## How It Works

```
source -> [lexer] -> flat token stream -> [parser] -> syntax tree -> [interpreter] -> output
```

## To Do 
- Static methods
- Getter methods
- Arrays and other data structures
