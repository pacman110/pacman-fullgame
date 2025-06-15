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

            // evento tecla pressionada
            else if (event.type == sf::Event::KeyPressed) {
                std::cout << "Tecla " << event.key.code << " pressionada\n";

                // quando tecla ESC pressionada, informa estado das teclas modificadoras
                if (event.key.code == sf::Keyboard::Escape) {
                    std::cout << "Tecla ESC pressionada" << std::endl;
                    std::cout << "control:" << event.key.control << std::endl;
                    std::cout << "alt:" << event.key.alt << std::endl;
                    std::cout << "shift:" << event.key.shift << std::endl;
                    std::cout << "system:" << event.key.system << std::endl;

                    if(event.key.shift)
                    window.close();
                }
            }
        }
    }

    return 0;
}
