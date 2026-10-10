class Person {
    info() {
        var name = "Daniel";
        print "Your age is " + this.age + " and your name is " + name;
    }
}
var person = Person();
person.age = "19";
person.info();
