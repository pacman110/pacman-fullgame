#include <SFML/Graphics.hpp>

int main() {
    // cria a janela
    sf::RenderWindow window(sf::VideoMode(800, 600), "Minha janela");

    // cria um círculo de raio 50
    sf::CircleShape circ(50);
    // define a posição absoluta do círculo
    circ.setPosition(10, 50);
    // define a cor do círculo (verde)
    circ.setFillColor(sf::Color(100, 250, 50));

    // cria um quadrado de tamanho 50
    sf::RectangleShape quad(sf::Vector2f(50, 50));
    quad.setFillColor(sf::Color(200, 200, 00));

    float posx = 375, posy = 275;   //posicao do quadrado

    // executa o programa enquanto a janela está aberta
    while (window.isOpen()) {

        // verifica todos os eventos que foram acionados na janela desde a última iteração do loop
        sf::Event event;
        while (window.pollEvent(event)) {
            // evento "fechar" acionado: fecha a janela
            if (event.type == sf::Event::Closed)
                window.close();
            // tecla pressionada
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Left)
                    posx -= 10;   // left key: move o quadrado para esquerda
                else if (event.key.code == sf::Keyboard::Right)
                    posx += 10;   // right key: move o quadrado para direita
            }
        }

        // limpa a janela com a cor preta
        window.clear(sf::Color::Black);

        // desenhar tudo aqui...

        //reposiciona o quadrado
        quad.setPosition(posx, posy);

        // desenha o círculo na janela
        window.draw(circ);
        // desenha o quadrado na janela
        window.draw(quad);

        // termina e desenha o frame corrente
        window.display();
    }

    return 0;
}
