#include <SFML/Graphics.hpp>
#include <iostream>

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

    // cria uma "textura" (uma figura)
    sf::Texture texture;
    if (!texture.loadFromFile("1up.png")) {
        std::cout << "Erro lendo imagem 1up.png\n";
        return 0;
    }
    // cria um sprite (um objeto gráfico) com a figura
    sf::Sprite sprite;
    sprite.setTexture(texture);

    float posxup = 500, posyup = 50;   //posicao inicial do sprite de vida

    // cria um relogio para medir o tempo
    sf::Clock clock;

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

        // Muda a posição do sprite vida a cada 0.1 segundos
        if (clock.getElapsedTime() > sf::seconds(0.1)) { // tempo desde último restart > 0.1s?
            clock.restart();      // recomeça contagem do tempo
            posxup += 10;         // muda posição de desenho da vida
        }

        // limpa a janela com a cor preta
        window.clear(sf::Color::Black);

        // desenhar tudo aqui...

        //reposiciona o quadrado e a vida
        quad.setPosition(posx, posy);
        sprite.setPosition(posxup, posyup);

        // desenha o círculo na janela
        window.draw(circ);
        // desenha o quadrado na janela
        window.draw(quad);
        // desenha sprite de vida na janela
        window.draw(sprite);

        // termina e desenha o frame corrente
        window.display();
    }

    return 0;
}
