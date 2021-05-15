#include <iostream>

int main(){
    int pc = 16;

    if(pc & 4){
        std::cout << "enpassant" << std::endl;
    }else if(pc & 1){
        std::cout << "capture" << std::endl;
    }else{
        std::cout << "move" << std::endl;
    }
    return 0;
}