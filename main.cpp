#include <SFML/Audio.hpp>  // <-- Adicionado para áudio
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

int posx = 1;       // posicao do PacMan
int posy = 29;

// posicao ghost
int ghostX = 13;
int ghostY = 15; 

void defineGhost(sf::Sprite &ghost1,  sf::Sprite &ghost2,  sf::Sprite &ghost3,  sf::Sprite &ghost4);


int main() {
    // cria a janela
    sf::RenderWindow window(sf::VideoMode(980, 1085), "Potter-Man");

    // shape da parede
    sf::RectangleShape rectangle(sf::Vector2f(SIZE, SIZE));
    rectangle.setFillColor(sf::Color(0, 255, 255));
    rectangle.setOutlineThickness(-5);
    rectangle.setOutlineColor(sf::Color(50, 50, 50));

     // // sprite do PacMan
    sf::Texture texturePac;
    if (!texturePac.loadFromFile("resources/hp.png")) {
       std::cout << "Erro lendo imagem hp.png\n";
        return 0;
    }
    sf::Sprite pac; // Criando objeto spitepac
    pac.setTexture(texturePac); // atribuindo a textura
    sf::Vector2u tamanhoPac = texturePac.getSize(); // Pega o taamnho real da imagem (Textura do pacman)
    pac.setOrigin(tamanhoPac.x / 2.5f, tamanhoPac.y / 2.5f); // origem = centro // Divide o tamanho da celula para ir para o centro (f = float)
    // Tamanho do pacman
    pac.setScale(1.5f,1.5f);


    bool moveesquerda = false;

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
    pedra.setScale(0.8f, 0.8f);

    sf::Sprite ghost1; // ✅ Criando objeto sprite ghost4
    sf::Sprite ghost2; // ✅ Criando objeto sprite ghost4
    sf::Sprite ghost3; // ✅ Criando objeto sprite ghost4
    sf::Sprite ghost4; // ✅ Criando objeto sprite ghost4
   

    /*
     // Sprites dos dementadores (ghost) - Textura
     //Ghost1
    sf::Texture texturaGhost1; // ✅ Criando o objeto textura do ghost
    if (!texturaGhost1.loadFromFile("resources/ghost1.png")) {
       std::cout << "Erro lendo imagem ghost1.png\n";
    return 0;
    }
    sf::Vector2u tamanhoGhost1 = texturaGhost1.getSize(); // ✅ Pega o tamanho real
    sf::Sprite ghost1; // ✅ Criando objeto sprite ghost1
    ghost1.setTexture(texturaGhost1);
    ghost1.setOrigin(tamanhoGhost1.x / 2.0f, tamanhoGhost1.y / 2.0f);
    ghost1.setScale(1.3f, 1.3f);

    //Ghost2

    sf::Texture texturaGhost2; // ✅ Criando o objeto textura do ghost
    if (!texturaGhost2.loadFromFile("resources/ghost2.png")) {
       std::cout << "Erro lendo imagem ghost2.png\n";
    return 0;
    }
    sf::Vector2u tamanhoGhost2 = texturaGhost2.getSize(); // ✅ Pega o tamanho real
    sf::Sprite ghost2; // ✅ Criando objeto sprite ghost2
    ghost2.setTexture(texturaGhost2);
    ghost2.setOrigin(tamanhoGhost2.x / 2.0f, tamanhoGhost2.y / 2.0f);
    ghost2.setScale(1.3f, 1.3f);
    
    //Ghost3

    sf::Texture texturaGhost3; // ✅ Criando o objeto textura do ghost
    if (!texturaGhost3.loadFromFile("resources/ghost3.png")) {
       std::cout << "Erro lendo imagem ghost3.png\n";
    return 0;
    }
    sf::Vector2u tamanhoGhost3 = texturaGhost3.getSize(); // ✅ Pega o tamanho real
    sf::Sprite ghost3; // ✅ Criando objeto sprite ghost3
    ghost3.setTexture(texturaGhost3);
    ghost3.setOrigin(tamanhoGhost3.x / 2.0f, tamanhoGhost3.y / 2.0f);
    ghost3.setScale(1.3f, 1.3f);

    //Ghost4

    sf::Texture texturaGhost4; // ✅ Criando o objeto textura do ghost
    if (!texturaGhost4.loadFromFile("resources/ghost4.png")) {
       std::cout << "Erro lendo imagem ghost4.png\n";
    return 0;
    }
    sf::Vector2u tamanhoGhost4 = texturaGhost4.getSize(); // ✅ Pega o tamanho real
    sf::Sprite ghost4; // ✅ Criando objeto sprite ghost4
    ghost4.setTexture(texturaGhost4);
    ghost4.setOrigin(tamanhoGhost4.x / 2.0f, tamanhoGhost4.y / 2.0f);
    ghost4.setScale(1.3f, 1.3f);

*/

    defineGhost(ghost1, ghost2, ghost3, ghost4);


    // Carrega textura do fundo único
    sf::Texture texturaFundo;  // Criando o objeto textura do fundo do jogo
    if (!texturaFundo.loadFromFile("resources/tiles/fundo.png")) {
        std::cout << "Erro lendo imagem fundo.png\n";
        return 0;
    }
    sf::Sprite spriteFundo; // Criando objeto sprite fundo
    spriteFundo.setTexture(texturaFundo);

    sf::Texture texturaParede;  // Criando o objeto textura da parede do jogo
    if (!texturaParede.loadFromFile("resources/tiles/parede.png")) {
        std::cout << "Erro lendo imagem parede.png\n";
        return 0;
    }
    sf::Sprite spriteParede; // Criando objeto sprite fundo
    spriteParede.setTexture(texturaParede);
    // executa o programa enquanto a janela está aberta

    // Música de fundo
    sf::Music music;
      if (!music.openFromFile("resources/hp_sound.ogg")) {
    std::cout << "Erro ao carregar a música\n";
    return 0;
    }
    music.setLoop(true);  // Repetir a música
    music.setVolume(100);    // Volume de 0 a 100 (pode ajustar para menos se estiver muito alto)
    music.play();         // Começa a tocar


    while (window.isOpen()) {

        // verifica todos os eventos que foram acionados na janela desde a última iteração do loop
        sf::Event event;
        while (window.pollEvent(event)) {
            // evento "fechar" acionado: fecha a janela
            if (event.type == sf::Event::Closed)
                window.close();

            // tecla pressionada
            if (event.type == sf::Event::KeyPressed) {
               if (event.key.code == sf::Keyboard::Left && mapa [posy][posx-1] != '1'){
                  posx--;   // left key: move o PacMan para esquerda
                  moveesquerda = true;
               }
               else if (event.key.code == sf::Keyboard::Right && mapa [posy] [posx+1] !='1'){ 
                  posx++;   // right key: move o PacMan para direita
                  moveesquerda = false;
               }
               else if (event.key.code == sf::Keyboard::Up)
                  posy--;   // up key: move o PacMan para cima
               else if (event.key.code == sf::Keyboard::Down)
                  posy++;   // down key: move o PacMan para baixo
            }
        }
    

        // limpa a janela com a cor preta
    window.clear(sf::Color::Black);
    // desenha o fundo da tela (imagem única)
    window.draw(spriteFundo);


        // desenhar tudo aqui...

        // desenha paredes
        for(int i=0;i<31;i++)
            for(int j=0;j<29;j++){
                if (mapa[i][j]=='1') {
                   spriteParede.setPosition(j*SIZE, i*SIZE);
                    window.draw(spriteParede);
                }
                else if(mapa[i][j] == '2'){
                    // Pega a posição da coluna e da linha, ajusta para o centro
                    pedra.setPosition(j*SIZE + SIZE/2.0f, i*SIZE + SIZE/2.0f); 
                    window.draw(pedra);
                }
            }
        // desenha o PacMan na posição atual
        pac.setPosition(posx * SIZE + SIZE / 2.0f, posy * SIZE + SIZE / 2.0f);
        ghost1.setPosition(ghostX * SIZE + SIZE / 2.0f, (ghostY-2) * SIZE + SIZE / 2.0f);
        ghost2.setPosition((ghostX-2) * SIZE + SIZE / 2.0f, (ghostY-1) * SIZE + SIZE / 2.0f);
        ghost3.setPosition((ghostX+2) * SIZE + SIZE / 2.0f, (ghostY-1) * SIZE + SIZE / 2.0f);
        ghost4.setPosition(ghostX * SIZE + SIZE / 2.0f, ghostY * SIZE + SIZE / 2.0f);
        window.draw(pac);

        window.draw(ghost1);
        window.draw(ghost2);
        window.draw(ghost3);
        window.draw(ghost4);
       
        // termina e desenha o frame corrente
        window.display();
        
        }
    return 0;
}



void defineGhost( sf::Sprite &ghost1,  sf::Sprite &ghost2,  sf::Sprite &ghost3,  sf::Sprite &ghost4) {

         // Sprites dos dementadores (ghost) - Textura
     //Ghost1
    sf::Texture texturaGhost1; // ✅ Criando o objeto textura do ghost
    if (!texturaGhost1.loadFromFile("resources/ghost1.png")) {
       std::cout << "Erro lendo imagem ghost1.png\n";
    //return 0;
    }
    sf::Vector2u tamanhoGhost1 = texturaGhost1.getSize(); // ✅ Pega o tamanho real
    //sf::Sprite ghost1; // ✅ Criando objeto sprite ghost1
    ghost1.setTexture(texturaGhost1);
    ghost1.setOrigin(tamanhoGhost1.x / 2.0f, tamanhoGhost1.y / 2.0f);
    ghost1.setScale(1.3f, 1.3f);

    //Ghost2

    sf::Texture texturaGhost2; // ✅ Criando o objeto textura do ghost
    if (!texturaGhost2.loadFromFile("resources/ghost2.png")) {
       std::cout << "Erro lendo imagem ghost2.png\n";
    //return 0;
    }
    sf::Vector2u tamanhoGhost2 = texturaGhost2.getSize(); // ✅ Pega o tamanho real
    //sf::Sprite ghost2; // ✅ Criando objeto sprite ghost2
    ghost2.setTexture(texturaGhost2);
    ghost2.setOrigin(tamanhoGhost2.x / 2.0f, tamanhoGhost2.y / 2.0f);
    ghost2.setScale(1.3f, 1.3f);
    
    //Ghost3

    sf::Texture texturaGhost3; // ✅ Criando o objeto textura do ghost
    if (!texturaGhost3.loadFromFile("resources/ghost3.png")) {
       std::cout << "Erro lendo imagem ghost3.png\n";
   // return 0;
    }
    sf::Vector2u tamanhoGhost3 = texturaGhost3.getSize(); // ✅ Pega o tamanho real
    //sf::Sprite ghost3; // ✅ Criando objeto sprite ghost3
    ghost3.setTexture(texturaGhost3);
    ghost3.setOrigin(tamanhoGhost3.x / 2.0f, tamanhoGhost3.y / 2.0f);
    ghost3.setScale(1.3f, 1.3f);

    //Ghost4

    sf::Texture texturaGhost4; // ✅ Criando o objeto textura do ghost
    if (!texturaGhost4.loadFromFile("resources/ghost4.png")) {
       std::cout << "Erro lendo imagem ghost4.png\n";
   // return 0;
    }
    sf::Vector2u tamanhoGhost4 = texturaGhost4.getSize(); // ✅ Pega o tamanho real
    //sf::Sprite ghost4; // ✅ Criando objeto sprite ghost4
    ghost4.setTexture(texturaGhost4);
    ghost4.setOrigin(tamanhoGhost4.x / 2.0f, tamanhoGhost4.y / 2.0f);
    ghost4.setScale(1.3f, 1.3f);

}