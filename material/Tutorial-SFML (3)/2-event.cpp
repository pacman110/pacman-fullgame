#include <SFML/Window.hpp>
#include <iostream>

int main() {
    // cria a janela
    sf::Window window(sf::VideoMode(800, 600), "Minha janela");

    // executa o programa enquanto a janela está aberta
    while (window.isOpen()) {

        // verifica todos os eventos que foram acionados na janela desde a última iteração do loop
        sf::Event event;

        // enquanto houver evento pendente
        while (window.pollEvent(event)) {

          // verifica o tipo do evento...

          // evento "fechar" acionado: fecha a janela
        if (event.type == sf::Event::Closed)
            window.close();
        else if (event.type == sf::Event::KeyReleased)
            std::cout << "Tecla solta: " << event.key.code << std::endl;

          // evento tecla pressionada
        else if (event.type == sf::Event::KeyPressed)
            std::cout << "Tecla " << event.key.code << " pressionada\n";
        }
    }

    return 0;
}
