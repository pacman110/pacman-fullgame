#include <SFML/Graphics.hpp>
#include <iostream>

// Código base para jogo do Pac-Man usando SFML
// Mapa desenhado:        André Gustavo   15/06/23
// Movimentos Pac-Man:    André Gustavo   15/06/23
// Movimento contínuo:    André Gustavo   16/06/23
// Colisão com paredes:   não implementada
// Intenção de movimento: não implementado

const int SIZE = 50;      // Tamanho de cada célula do mapa

char mapa[11][21] = {     // Mapa do jogo
  "11111111111111111111",
  "10000100000000100001",
  "10110101111110101101",
  "10100000000000000101",
  "10101101100110110101",
  "10000001000010000001",
  "10101101111110110101",
  "10100000000000000101",
  "10110101111110101101",
  "10000100000000100001",
  "11111111111111111111"
};

int posx = 9;       // posicao do PacMan
int posy = 7;

bool cima = false;  // direcao de movimento do PacMan
bool baixo = false;
bool esq = false;
bool dir = false;

int main() {
    // cria a janela
    sf::RenderWindow window(sf::VideoMode(1000, 550), "Pac-Man");

    // shape da parede
    sf::RectangleShape rectangle(sf::Vector2f(SIZE, SIZE));
    rectangle.setFillColor(sf::Color(0, 255, 255));
    rectangle.setOutlineThickness(-5);
    rectangle.setOutlineColor(sf::Color(50, 50, 50));

    // sprite do PacMan
    sf::Texture texture;
    if (!texture.loadFromFile("pacman.png")) {
        std::cout << "Erro lendo imagem pacman.png\n";
        return 0;
    }
    sf::Sprite sprite;
    sprite.setTexture(texture);

    // cria um relogio para medir o tempo do PacMan
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
              if (event.key.code == sf::Keyboard::Left) {
                  esq = true;   // left key: PacMan passa a mover para esquerda
                  dir = cima = baixo = false;
              }
              else if (event.key.code == sf::Keyboard::Right) {
                  dir = true;   // right key: PacMan passa a mover para direita
                  esq = cima = baixo = false;
              }
              else if (event.key.code == sf::Keyboard::Up) {
                  cima = true;  // up key: PacMan passa a mover para cima
                  esq = dir = baixo = false;
              }
              else if (event.key.code == sf::Keyboard::Down) {
                  baixo = true; // down key:PacMan passa a mover para baixo
                  esq = dir = cima = false;
              }
            }
        }

        // Muda a posição do PacMan a cada 0.2 segundos
        if (clock.getElapsedTime() > sf::seconds(0.2)) { // tempo desde último restart > 0.2s?
            clock.restart();      // recomeça contagem do tempo
            if (cima) posy--;     // muda a posição de acordo com booleano ativo
            if (baixo) posy++;
            if (esq) posx--;
            if (dir) posx++;
        }

        // limpa a janela com a cor preta
        window.clear(sf::Color::Black);

        // desenhar tudo aqui...

        // desenha paredes
        for(int i=0;i<11;i++)
            for(int j=0;j<21;j++)
                if (mapa[i][j]=='1') {
                    rectangle.setPosition(j*SIZE, i*SIZE);
                    window.draw(rectangle);
                }

        // desenha PacMan
        sprite.setPosition(posx*SIZE,posy*SIZE);
        window.draw(sprite);

        // termina e desenha o frame corrente
        window.display();
    }

    return 0;
}
