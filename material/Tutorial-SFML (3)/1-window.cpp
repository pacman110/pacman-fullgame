#include <SFML/Window.hpp>
#include <iostream>
using namespace std;
int main() {
    // cria a janela
    sf::Window window(sf::VideoMode(800, 600), "Minha janela");

    // executa o programa enquanto a janela está aberta
    while (window.isOpen()) {

        // verifica todos os eventos que foram acionados na janela desde a última iteração do loop
        sf::Event event;
        while (window.pollEvent(event)) {
            // evento "fechar" acionado: fecha a janela
            if (event.type == sf::Event::Closed)
                window.close();
        }
    }

    return 0;
}
