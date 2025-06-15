#include <SFML/Graphics.hpp>
#include <iostream>

char mapa[9][17] = {    // Mapa do jogo
  "1111111111111111",
  "1000010000100001",
  "1011000110001101",
  "1011010110101101",
  "1000010000100001",
  "1011010110101101",
  "1011000000001101",
  "1000011111100001",
  "1111111111111111"
};

int posx = 7; // posicao do PacMan
int posy = 6;

int main() {
    // cria a janela
    sf::RenderWindow window(sf::VideoMode(800, 600), "Pac-Man");

    // shape da parede
    sf::RectangleShape rectangle(sf::Vector2f(50, 50));
    rectangle.setFillColor(sf::Color(0, 255, 255));

    // sprites do PacMan
    sf::Texture texture;
    if (!texture.loadFromFile("pacman.png")) {
        std::cout << "Erro lendo imagem pacman.png\n";
        return 0;
    }
    sf::Sprite sprite;
    sprite.setTexture(texture);

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
                    posx--;   // left key: move o PacMan para esquerda
                else if (event.key.code == sf::Keyboard::Right)
                    posx++;   // right key: move o PacMan para direita
                else if (event.key.code == sf::Keyboard::Up)
                    posy--;   // up key: move o PacMan para cima
                else if (event.key.code == sf::Keyboard::Down)
                    posy++;   // down key: move o PacMan para baixo
            }
        }

        // limpa a janela com a cor preta
        window.clear(sf::Color::Black);

        // desenhar tudo aqui...

        // desenha paredes
        for(int i=0;i<9;i++)
            for(int j=0;j<17;j++)
                if (mapa[i][j]=='1') {
                    rectangle.setPosition(j*50, i*50);
                    window.draw(rectangle);
                }

        // desenha PacMan
        sprite.setPosition(posx*50,posy*50);
        window.draw(sprite);

        // termina e desenha o frame corrente
        window.display();
    }

    return 0;
}
