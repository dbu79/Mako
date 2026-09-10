#include <iostream>

int main(){
    while (true){
        std::string text = "";
        std::cout << "basic > ";
        std::getline(std::cin, text);
        std::cout << text << std::endl;
    }
}



