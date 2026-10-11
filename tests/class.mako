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

var person2 = Person2("Matthew", "21");
person2.info();
