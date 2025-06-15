#include <SFML/Graphics.hpp>
#include <iostream>

// Código base para jogo do Pac-Man usando SFML
// Mapa desenhado:        André Gustavo   15/06/23
// Movimentos Pac-Man:    André Gustavo   15/06/23
// Movimento contínuo:    não implementado
// Colisão com paredes:   não implementada
// Intenção de movimento: não implementado

const int SIZE = 35;      // Tamanho de cada célula do mapa

char mapa[31][29] = {
    "1111111111111111111111111111",
    "1222222222222222222222222221",
    "1211211112111111112111121121",
    "1211211112111111112111121121",
    "1211222222112222112222221121",
    "1211222112112112112112221121",
    "1211112112112112112112111121",
    "1211112112112112112112111121",
    "1222222112222112222112222221",
    "1112111111112112111111112111",
    "1112111111112112111111112111",
    "1112222110000000000112222111",
    "2222112110111001110112112222",
    "1111112110100000010112111111",
    "1111112220100000010222111111",
    "2222112110100000010112112222",
    "1112112110111111110112112111",
    "1112222110000000000112222111",
    "1112112111112112111112112111",
    "1112112111112112111112112111",
    "1112112222222112222222112111",
    "1112111112112112112111112111",
    "1112111112112112112111112111",
    "1222222222112222112222222221",
    "1211112112111111112112111121",
    "1211112112111111112112111121",
    "1211222112222222222112221121",
    "1211211111112112111111121121",
    "1211211111112112111111121121",
    "1022222222222112222222222221",
    "1111111111111111111111111111",
};

int posx = 9;       // posicao do PacMan
int posy = 7;

int main() {
    // cria a janela
    sf::RenderWindow window(sf::VideoMode(980, 1085), "Potter-Man");

    // shape da parede
    sf::RectangleShape rectangle(sf::Vector2f(SIZE, SIZE));
    rectangle.setFillColor(sf::Color(0, 255, 255));
    rectangle.setOutlineThickness(-5);
    rectangle.setOutlineColor(sf::Color(50, 50, 50));

    // // sprite do PacMan
    // sf::Texture texture;
    // if (!texture.loadFromFile("pacman.png")) {
    //     std::cout << "Erro lendo imagem pacman.png\n";
    //     return 0;
    // }
    // sf::Sprite sprite;
    // sprite.setTexture(texture);
    
    // Sprite da Pedra (Frutinha) -Textura
    sf::Texture texturaPedra; // Criando o objeto textura da pedra
    if (!texturaPedra.loadFromFile("resources/pedra.png")) {
            std::cout << "Erro lendo imagem pedra.png\n";
        return 0;
    } //
    sf::Vector2u tamanhoTextura = texturaPedra.getSize(); // Pega o taamnho real da imagem (Textura da pedra)
    sf::Sprite pedra; // Criando objeto spitepedra
    pedra.setTexture(texturaPedra); // atribuindo a textura
    pedra.setOrigin(tamanhoTextura.x / 2.0f, tamanhoTextura.y / 2.0f); // origem = centro // Divide o tamanho da celula para ir para o centro (f = float)

    // executa o programa enquanto a janela está aberta
    while (window.isOpen()) {

        // verifica todos os eventos que foram acionados na janela desde a última iteração do loop
        sf::Event event;
        while (window.pollEvent(event)) {
            // evento "fechar" acionado: fecha a janela
            if (event.type == sf::Event::Closed)
                window.close();

            // tecla pressionada
            // if (event.type == sf::Event::KeyPressed) {
            //     if (event.key.code == sf::Keyboard::Left)
            //         posx--;   // left key: move o PacMan para esquerda
            //     else if (event.key.code == sf::Keyboard::Right)
            //         posx++;   // right key: move o PacMan para direita
            //     else if (event.key.code == sf::Keyboard::Up)
            //         posy--;   // up key: move o PacMan para cima
            //     else if (event.key.code == sf::Keyboard::Down)
            //         posy++;   // down key: move o PacMan para baixo
            // }
        }

        // limpa a janela com a cor preta
        window.clear(sf::Color::Black);

        // desenhar tudo aqui...

        // desenha paredes
        for(int i=0;i<31;i++)
            for(int j=0;j<29;j++){
                if (mapa[i][j]=='1') {
                    rectangle.setPosition(j*SIZE, i*SIZE);
                    window.draw(rectangle);
                }
                else if(mapa[i][j] == '2'){
                    // Pega a posição da coluna e da linha, ajusta para o centro
                    pedra.setPosition(j*SIZE + SIZE/2.0f, i*SIZE + SIZE/2.0f); 
                    window.draw(pedra);
                }
            }
        // desenha PacMan
        // sprite.setPosition(posx*SIZE,posy*SIZE);
        // window.draw(sprite);

        // termina e desenha o frame corrente
        window.display();
    }

    return 0;
}
