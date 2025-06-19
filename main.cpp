#include <SFML/Audio.hpp>  // <-- Adicionado para áudio
#include <SFML/Graphics.hpp>
#include <iostream>


// Código base para jogo do Pac-Man usando SFML
// Mapa desenhado:        André Gustavo   15/06/23
// Movimentos Pac-Man:    
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
double ghost1X = 11.2;
double ghost1Y = 14; 

double ghost2X = 12.5;
double ghost2Y = 14; 

double ghost3X = 14;
double ghost3Y = 14; 

double ghost4X = 15.5;
double ghost4Y = 14; 

//verificar se os dementadores ainda estão dentro da caixa inicial
bool ghost1Preso = true;
bool ghost2Preso = true;
bool ghost3Preso = true;
bool ghost4Preso = true;

// Direção atual dos fantasmas
int dirGhost1 = 0; //  em cima 
int dirGhost2 = 1; // em baixo
int dirGhost3 = 2; // a esquerda
int dirGhost4 = 3; // a direita

// Clocks para controlar tempo de movimento dos fantasmas
sf::Clock clockGhost1;
sf::Clock clockGhost2;
sf::Clock clockGhost3;
sf::Clock clockGhost4;


// PROTOTIPO DE FUNCOES

void defineGhost(sf::Sprite &ghost1, sf::Sprite &ghost1_left, sf::Sprite &ghost1_up, sf::Sprite &ghost1_down,
                 sf::Sprite &ghost2, sf::Sprite &ghost2_left, sf::Sprite &ghost2_up, sf::Sprite &ghost2_down,
                 sf::Sprite &ghost3, sf::Sprite &ghost3_left, sf::Sprite &ghost3_up, sf::Sprite &ghost3_down,
                 sf::Sprite &ghost4, sf::Sprite &ghost4_left, sf::Sprite &ghost4_up, sf::Sprite &ghost4_down);

// Função que verifica se uma coordenada (x, y) é válida para movimentação
// Retorna true se for dentro dos limites do mapa e não for parede

bool podeMover(int x, int y) {                                        // Verifica se x está entre 0 e 28 (colunas válidas do mapa)
   return x >= 0 && x < 29 && y >= 0 && y < 31 && mapa[y][x] != '1';  // Verifica se y está entre 0 e 30 (linhas válidas do mapa)
}                                                                     // Verifica se o caractere naquela posição não é '1' (parede)
                                                                      
    
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
    pac.setScale(1.3f,1.3f);

   // Sprite do PacMan para cima (hp_up)
   sf::Texture texturePac_up;
   if (!texturePac_up.loadFromFile("resources/hp_up.png")) {
      std::cout << "Erro lendo imagem hp_up.png\n";
       return 0;
   }
   sf::Sprite pac_up;
   pac_up.setTexture(texturePac_up);
   sf::Vector2u tamanhoPac_up = texturePac_up.getSize();
   pac_up.setOrigin(tamanhoPac_up.x / 2.5f, tamanhoPac_up.y / 2.5f);
   pac_up.setScale(1.3f, 1.3f);

   // Sprite do PacMan para baixo (hp_down)
   sf::Texture texturePac_down;
   if (!texturePac_down.loadFromFile("resources/hp_down.png")) {
   std::cout << "Erro lendo imagem hp_down.png\n";
    return 0;
   }
   sf::Sprite pac_down;
   pac_down.setTexture(texturePac_down);
   sf::Vector2u tamanhoPac_down = texturePac_down.getSize();
   pac_down.setOrigin(tamanhoPac_down.x / 2.5f, tamanhoPac_down.y / 2.5f);
   pac_down.setScale(1.3f, 1.3f);

   // Sprite do PacMan para esquerda (hp_left)
   sf::Texture texturePac_left;
   if (!texturePac_left.loadFromFile("resources/hp_left.png")) {
   std::cout << "Erro lendo imagem hp_left.png\n";
    return 0;
   }
   sf::Sprite pac_left;
   pac_left.setTexture(texturePac_left);
   sf::Vector2u tamanhoPac_left = texturePac_left.getSize();
   pac_left.setOrigin(tamanhoPac_left.x / 2.5f, tamanhoPac_left.y / 2.5f);
   pac_left.setScale(1.3f, 1.3f);

   // Sprite do PacMan para direita (hp_right)
   sf::Texture texturePac_right;
   if (!texturePac_right.loadFromFile("resources/hp_right.png")) {
   std::cout << "Erro lendo imagem hp_right.png\n";
    return 0;
   }
   sf::Sprite pac_right;
   pac_right.setTexture(texturePac_right);
   sf::Vector2u tamanhoPac_right = texturePac_right.getSize();
   pac_right.setOrigin(tamanhoPac_right.x / 2.5f, tamanhoPac_right.y / 2.5f);
   pac_right.setScale(1.3f, 1.3f);
   

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

    sf::Sprite ghost1;        // ✅ Criando objeto sprite ghost1
    sf::Sprite ghost1_left;
    sf::Sprite ghost1_up;
    sf::Sprite ghost1_down;

    sf::Sprite ghost2;        // ✅ Criando objeto sprite ghost2
    sf::Sprite ghost2_left;
    sf::Sprite ghost2_up;
    sf::Sprite ghost2_down;

    sf::Sprite ghost3;        // ✅ Criando objeto sprite ghost3
    sf::Sprite ghost3_left;
    sf::Sprite ghost3_up;
    sf::Sprite ghost3_down;

    sf::Sprite ghost4;        // ✅ Criando objeto sprite ghost4
    sf::Sprite ghost4_left;
    sf::Sprite ghost4_up;
    sf::Sprite ghost4_down;

    // Função para definir o sprite dos dementadores
    defineGhost(ghost1, ghost1_left, ghost1_up, ghost1_down,
               ghost2, ghost2_left, ghost2_up, ghost2_down,
               ghost3, ghost3_left, ghost3_up, ghost3_down,
               ghost4, ghost4_left, ghost4_up, ghost4_down);


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
               if (event.key.code == sf::Keyboard::Left)
                  pac = pac_left;
               if (event.key.code == sf::Keyboard::Left && mapa [posy][posx-1] != '1'){
                  posx--;   // left key: move o PacMan para esquerda
                  moveesquerda = true;
               }
               else if (event.key.code == sf::Keyboard::Right && mapa [posy] [posx+1] !='1'){
                  pac = pac_right; 
                  posx++;   // right key: move o PacMan para direita
                  moveesquerda = false;
               }   
               else if (event.key.code == sf::Keyboard::Up && mapa [posy-1][posx] != '1' ){
                  pac = pac_up;
                  posy--;   // up key: move o PacMan para cima  
               }
               else if (event.key.code == sf::Keyboard::Down && mapa [posy+1][posx] != '1' ){
                  pac = pac_down;
                  posy++;   // down key: move o PacMan para baixo
               }
            }
        }

    //(teletransporte do pac de um lado para outo)
    if (posx < 0) posx = 27;     
    if (posx > 27) posx = 0; 

    //Movimento Ghost1 aleatório//
    
    // Movimento ghost1: aleatório com atraso de 1s
    if (ghost1Preso) {
       if (clockGhost1.getElapsedTime().asSeconds() > 1.0f && podeMover(ghost1X, ghost1Y - 1)) {
          ghost1Y -= 1;
          ghost1 = ghost1_up;
          if ((int)ghost1Y < 13) ghost1Preso = false;
             clockGhost1.restart();
       }
    } 
    else {
    // Verifica se já passou 0.2 segundos desde o último movimento do fantasma 1
    if (clockGhost1.getElapsedTime().asSeconds() > 0.2f) {
    // Define deslocamentos para cima, baixo, esquerda e direita
       int dx[] = {0, 0, -1, 1};    // variação em X para cada direção
       int dy[] = {-1, 1, 0, 0};    // variação em Y para cada direção

    // Sorteia uma direção aleatória entre 0 e 3
       int novaDirecao = rand() % 4;

    // Calcula posição tentativa com base na direção sorteada
       int proxX = ghost1X + dx[novaDirecao];
       int proxY = ghost1Y + dy[novaDirecao];

    // Verifica se a nova posição não é uma parede
       if (podeMover(proxX, proxY)) {
        // Atualiza posição se for possível
           ghost1X = proxX;
           ghost1Y = proxY;
           // muda o sprite conforme a direção sorteada
          if (novaDirecao == 0) ghost1 = ghost1_up;
          else if (novaDirecao == 1) ghost1 = ghost1_down;
          else if (novaDirecao == 2) ghost1 = ghost1_left;
        // direita (novaDirecao == 3): mantém sprite padrão ghost1
       }
    // Reinicia o relógio do fantasma 1 para contar novo intervalo
    clockGhost1.restart();
    }
    }

    //Movimento Ghost2 aleatório
    
    // Movimento ghost2: aleatório com atraso de 2s
    if (ghost1Preso) {
       if (clockGhost2.getElapsedTime().asSeconds() > 1.0f && podeMover(ghost2X, ghost2Y - 1)) {
          ghost2Y -= 1;
          ghost2 = ghost2_up;
          if ((int)ghost2Y < 13) ghost1Preso = false;
             clockGhost2.restart();
       }
    } 
    else {
    // Verifica se já passou 0.2 segundos desde o último movimento do fantasma 1
    if (clockGhost2.getElapsedTime().asSeconds() > 0.2f) {
    // Define deslocamentos para cima, baixo, esquerda e direita
       int dx[] = {0, 0, -1, 1};    // variação em X para cada direção
       int dy[] = {-1, 1, 0, 0};    // variação em Y para cada direção

    // Sorteia uma direção aleatória entre 0 e 3
       int novaDirecao = rand() % 4;

    // Calcula posição tentativa com base na direção sorteada
       int proxX = ghost2X + dx[novaDirecao];
       int proxY = ghost2Y + dy[novaDirecao];

    // Verifica se a nova posição não é uma parede
       if (podeMover(proxX, proxY)) {
        // Atualiza posição se for possível
           ghost2X = proxX;
           ghost2Y = proxY;

        // muda o sprite conforme a direção sorteada
           if (novaDirecao == 0) ghost2 = ghost2_up;
           else if (novaDirecao == 1) ghost2 = ghost2_down;
           else if (novaDirecao == 2) ghost2 = ghost2_left;
        // direita (novaDirecao == 3): mantém sprite padrão ghost2
       }

    // Reinicia o relógio do fantasma 1 para contar novo intervalo
    clockGhost2.restart();
    }
}

    //Movimento Ghost3 persegue pac priorizando primeiro x e depois y
    
    //Movimento ghost3: persegue (X depois Y), com atraso de 3s
    if (ghost3Preso) {
       if (clockGhost3.getElapsedTime().asSeconds() > 3.0f && podeMover(ghost3X, ghost3Y - 1)) {
          ghost3Y -= 1;
          ghost3 = ghost3_up;
          if ((int)ghost3Y < 13) ghost3Preso = false;
             clockGhost3.restart();
    }
    } else {
    // Verifica se já passou 0.2 segundos desde o último movimento do fantasma 1
    if (clockGhost3.getElapsedTime().asSeconds() > 0.2f) {
    // Inicializa deslocamentos em zero
       int dx = 0;
       int dy = 0;

    // Tenta alinhar primeiro no eixo X
       if ((int)ghost3X < posx && podeMover(ghost3X + 1, ghost3Y)) {
          dx = 1;  // move para direita
       }
       else if ((int)ghost3X > posx && podeMover(ghost3X - 1, ghost3Y)) {
          dx = -1; // move para esquerda
       }
    // Se já estiver alinhado no X, tenta no Y
       else if ((int)ghost3Y < posy && podeMover(ghost3X, ghost3Y + 1)) {
          dy = 1;  // move para baixo
       }
       else if ((int)ghost3Y > posy && podeMover(ghost3X, ghost3Y - 1)) {
       dy = -1; // move para cima
       }

       // atualiza sprite conforme direção
        if (dx == -1) ghost3 = ghost3_left;
        else if (dx == 1) /* direita */ ; // sprite padrão
        else if (dy == 1) ghost3 = ghost3_down;
        else if (dy == -1) ghost3 = ghost3_up;

    // Aplica movimento
       ghost3X += dx;
       ghost3Y += dy;

       clockGhost3.restart();
    }
}

    //Movimento Ghost4 persegue pac priorizando primeiro y e depois x
    
    if (ghost4Preso) {
       if (clockGhost4.getElapsedTime().asSeconds() > 4.0f && podeMover(ghost4X, ghost4Y - 1)) {
          ghost4Y -= 1;
          ghost4 = ghost4_up;
          if ((int)ghost4Y < 13) ghost4Preso = false;
           clockGhost4.restart();
    }
    } else {
    // Verifica se já passou 0.2 segundos desde o último movimento do fantasma 4

    if (clockGhost4.getElapsedTime().asSeconds() > 0.2f) {
       int dx = 0;
       int dy = 0;

    // Tenta alinhar primeiro no eixo Y
       if ((int)ghost4Y < posy && podeMover(ghost4X, ghost4Y + 1)) {
          dy = 1;  // move para baixo
       }
       else if ((int)ghost4Y > posy && podeMover(ghost4X, ghost4Y - 1)) {
          dy = -1; // move para cima
       }
    // Se já estiver alinhado no Y, tenta no X
       else if ((int)ghost4X < posx && podeMover(ghost4X + 1, ghost4Y)) {
          dx = 1;  // move para direita
       }
       else if ((int)ghost4X > posx && podeMover(ghost4X - 1, ghost4Y)) {
          dx = -1; // move para esquerda
       }

       // atualiza sprite conforme direção
        if (dx == -1) ghost4 = ghost4_left;
        else if (dx == 1) /* direita */ ; // sprite padrão
        else if (dy == 1) ghost4 = ghost4_down;
        else if (dy == -1) ghost4 = ghost4_up;

       ghost4X += dx;
       ghost4Y += dy;

       clockGhost4.restart();
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
        
        //desenha o ghost na posição atual

        ghost1.setPosition(ghost1X * SIZE + SIZE / 2.0f, ghost1Y * SIZE + SIZE / 2.0f);
        ghost2.setPosition(ghost2X * SIZE + SIZE / 2.0f, ghost2Y * SIZE + SIZE / 2.0f);
        ghost3.setPosition(ghost3X * SIZE + SIZE / 2.0f, ghost3Y * SIZE + SIZE / 2.0f);
        ghost4.setPosition(ghost4X * SIZE + SIZE / 2.0f, ghost4Y * SIZE + SIZE / 2.0f);

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



void defineGhost(sf::Sprite &ghost1, sf::Sprite &ghost1_left, sf::Sprite &ghost1_up, sf::Sprite &ghost1_down,
                 sf::Sprite &ghost2, sf::Sprite &ghost2_left, sf::Sprite &ghost2_up, sf::Sprite &ghost2_down,
                 sf::Sprite &ghost3, sf::Sprite &ghost3_left, sf::Sprite &ghost3_up, sf::Sprite &ghost3_down,
                 sf::Sprite &ghost4, sf::Sprite &ghost4_left, sf::Sprite &ghost4_up, sf::Sprite &ghost4_down) {

    // Sprites dos dementadores (ghost) - Textura

    //Ghost1

static sf::Texture texturaGhost1; // ✅ Criando o objeto textura do ghost1
if (!texturaGhost1.loadFromFile("resources/ghost1.png")) { // ✅ Carregando imagem ghost1
    std::cout << "Erro lendo imagem ghost1.png\n"; // ✅ Mensagem de erro
    // return 0;
}
static sf::Vector2u tamanhoGhost1 = texturaGhost1.getSize(); // ✅ Pega o tamanho real
//sf::Sprite ghost1; // ✅ Criando objeto sprite ghost1
ghost1.setTexture(texturaGhost1); // ✅ Define textura no sprite
ghost1.setOrigin(tamanhoGhost1.x / 2.0f, tamanhoGhost1.y / 2.0f); // ✅ Origem no centro
ghost1.setScale(1.3f, 1.3f); // ✅ Escala da imagem

//Ghost1_left

static sf::Texture texturaGhost1_left; // ✅ Criando textura para movimento à esquerda
if (!texturaGhost1_left.loadFromFile("resources/ghost1_left.png")) {
    std::cout << "Erro lendo imagem ghost1_left.png\n";
}
static sf::Vector2u tamanhoGhost1_left = texturaGhost1_left.getSize(); // ✅ Pega o tamanho real
//sf::Sprite ghost1_left; // ✅ Criando objeto sprite ghost1_left
ghost1_left.setTexture(texturaGhost1_left); // ✅ Define textura
ghost1_left.setOrigin(tamanhoGhost1_left.x / 2.0f, tamanhoGhost1_left.y / 2.0f); // ✅ Origem no centro
ghost1_left.setScale(1.3f, 1.3f); // ✅ Escala

//Ghost1_up

static sf::Texture texturaGhost1_up; // ✅ Criando textura para movimento para cima
if (!texturaGhost1_up.loadFromFile("resources/ghost1_up.png")) {
    std::cout << "Erro lendo imagem ghost1_up.png\n";
}
static sf::Vector2u tamanhoGhost1_up = texturaGhost1_up.getSize(); // ✅ Pega o tamanho real
//sf::Sprite ghost1_up; // ✅ Criando objeto sprite ghost1_up
ghost1_up.setTexture(texturaGhost1_up); // ✅ Define textura
ghost1_up.setOrigin(tamanhoGhost1_up.x / 2.0f, tamanhoGhost1_up.y / 2.0f); // ✅ Origem no centro
ghost1_up.setScale(1.3f, 1.3f); // ✅ Escala

//Ghost1_down

static sf::Texture texturaGhost1_down; // ✅ Criando textura para movimento para baixo
if (!texturaGhost1_down.loadFromFile("resources/ghost1_down.png")) {
    std::cout << "Erro lendo imagem ghost1_down.png\n";
}
static sf::Vector2u tamanhoGhost1_down = texturaGhost1_down.getSize(); // ✅ Pega o tamanho real
//sf::Sprite ghost1_down; // ✅ Criando objeto sprite ghost1_down
ghost1_down.setTexture(texturaGhost1_down); // ✅ Define textura
ghost1_down.setOrigin(tamanhoGhost1_down.x / 2.0f, tamanhoGhost1_down.y / 2.0f); // ✅ Origem no centro
ghost1_down.setScale(1.3f, 1.3f); // ✅ Escala

//Ghost2

static sf::Texture texturaGhost2; // ✅ Criando o objeto textura do ghost2
if (!texturaGhost2.loadFromFile("resources/ghost2.png")) {
    std::cout << "Erro lendo imagem ghost2.png\n";
}
static sf::Vector2u tamanhoGhost2 = texturaGhost2.getSize(); // ✅ Pega o tamanho real
//sf::Sprite ghost2;
ghost2.setTexture(texturaGhost2); // ✅ Define textura
ghost2.setOrigin(tamanhoGhost2.x / 2.0f, tamanhoGhost2.y / 2.0f); // ✅ Origem no centro
ghost2.setScale(1.3f, 1.3f); // ✅ Escala

//Ghost2_left

static sf::Texture texturaGhost2_left;
if (!texturaGhost2_left.loadFromFile("resources/ghost2_left.png")) {
    std::cout << "Erro lendo imagem ghost2_left.png\n";
}
static sf::Vector2u tamanhoGhost2_left = texturaGhost2_left.getSize(); // ✅ Tamanho real
ghost2_left.setTexture(texturaGhost2_left); // ✅ Define textura
ghost2_left.setOrigin(tamanhoGhost2_left.x / 2.0f, tamanhoGhost2_left.y / 2.0f); // ✅ Origem no centro
ghost2_left.setScale(1.3f, 1.3f); // ✅ Escala

//Ghost2_up

static sf::Texture texturaGhost2_up; // ✅ Criando textura para movimento para cima
if (!texturaGhost2_up.loadFromFile("resources/ghost2_up.png")) {
    std::cout << "Erro lendo imagem ghost2_up.png\n";
}
static sf::Vector2u tamanhoGhost2_up = texturaGhost2_up.getSize(); // ✅ Pega o tamanho real da imagem
ghost2_up.setTexture(texturaGhost2_up); // ✅ Define textura no sprite ghost2_up
ghost2_up.setOrigin(tamanhoGhost2_up.x / 2.0f, tamanhoGhost2_up.y / 2.0f); // ✅ Define origem no centro
ghost2_up.setScale(1.3f, 1.3f); // ✅ Aplica escala no sprite 

//Ghost2_down

static sf::Texture texturaGhost2_down; // ✅ Criando textura para movimento para baixo
if (!texturaGhost2_down.loadFromFile("resources/ghost2_down.png")) {
    std::cout << "Erro lendo imagem ghost2_down.png\n";
}
static sf::Vector2u tamanhoGhost2_down = texturaGhost2_down.getSize(); // ✅ Pega o tamanho real da imagem
ghost2_down.setTexture(texturaGhost2_down); // ✅ Define textura no sprite ghost2_down
ghost2_down.setOrigin(tamanhoGhost2_down.x / 2.0f, tamanhoGhost2_down.y / 2.0f); // ✅ Define origem no centro
ghost2_down.setScale(1.3f, 1.3f); // ✅ Aplica escala no sprite

//Ghost3

static sf::Texture texturaGhost3; // ✅ Criando textura do ghost3
if (!texturaGhost3.loadFromFile("resources/ghost3.png")) {
    std::cout << "Erro lendo imagem ghost3.png\n";
}
static sf::Vector2u tamanhoGhost3 = texturaGhost3.getSize(); // ✅ Pega o tamanho real da imagem
ghost3.setTexture(texturaGhost3); // ✅ Define textura no sprite ghost3
ghost3.setOrigin(tamanhoGhost3.x / 2.0f, tamanhoGhost3.y / 2.0f); // ✅ Define origem no centro
ghost3.setScale(1.3f, 1.3f); // ✅ Aplica escala no sprite

//Ghost3_left

static sf::Texture texturaGhost3_left; // ✅ Criando textura para movimento à esquerda
if (!texturaGhost3_left.loadFromFile("resources/ghost3_left.png")) {
    std::cout << "Erro lendo imagem ghost3_left.png\n";
}
static sf::Vector2u tamanhoGhost3_left = texturaGhost3_left.getSize(); // ✅ Pega o tamanho real da imagem
ghost3_left.setTexture(texturaGhost3_left); // ✅ Define textura no sprite ghost3_left
ghost3_left.setOrigin(tamanhoGhost3_left.x / 2.0f, tamanhoGhost3_left.y / 2.0f); // ✅ Define origem no centro
ghost3_left.setScale(1.3f, 1.3f); // ✅ Aplica escala no sprite

//Ghost3_up

static sf::Texture texturaGhost3_up; // ✅ Criando textura para movimento para cima
if (!texturaGhost3_up.loadFromFile("resources/ghost3_up.png")) {
    std::cout << "Erro lendo imagem ghost3_up.png\n";
}
static sf::Vector2u tamanhoGhost3_up = texturaGhost3_up.getSize(); // ✅ Pega o tamanho real da imagem
ghost3_up.setTexture(texturaGhost3_up); // ✅ Define textura no sprite ghost3_up
ghost3_up.setOrigin(tamanhoGhost3_up.x / 2.0f, tamanhoGhost3_up.y / 2.0f); // ✅ Define origem no centro
ghost3_up.setScale(1.3f, 1.3f); // ✅ Aplica escala no sprite

//Ghost3_down

static sf::Texture texturaGhost3_down; // ✅ Criando textura para movimento para baixo
if (!texturaGhost3_down.loadFromFile("resources/ghost3_down.png")) {
    std::cout << "Erro lendo imagem ghost3_down.png\n";
}
static sf::Vector2u tamanhoGhost3_down = texturaGhost3_down.getSize(); // ✅ Pega o tamanho real da imagem
ghost3_down.setTexture(texturaGhost3_down); // ✅ Define textura no sprite ghost3_down
ghost3_down.setOrigin(tamanhoGhost3_down.x / 2.0f, tamanhoGhost3_down.y / 2.0f); // ✅ Define origem no centro
ghost3_down.setScale(1.3f, 1.3f); // ✅ Aplica escala no sprite

//Ghost4

static sf::Texture texturaGhost4; // ✅ Criando textura do ghost4
if (!texturaGhost4.loadFromFile("resources/ghost4.png")) {
    std::cout << "Erro lendo imagem ghost4.png\n";
}
static sf::Vector2u tamanhoGhost4 = texturaGhost4.getSize(); // ✅ Pega o tamanho real da imagem
ghost4.setTexture(texturaGhost4); // ✅ Define textura no sprite ghost4
ghost4.setOrigin(tamanhoGhost4.x / 2.0f, tamanhoGhost4.y / 2.0f); // ✅ Define origem no centro
ghost4.setScale(1.3f, 1.3f); // ✅ Aplica escala no sprite

//Ghost4_left

static sf::Texture texturaGhost4_left; // ✅ Criando textura para movimento à esquerda
if (!texturaGhost4_left.loadFromFile("resources/ghost4_left.png")) {
    std::cout << "Erro lendo imagem ghost4_left.png\n";
}
static sf::Vector2u tamanhoGhost4_left = texturaGhost4_left.getSize(); // ✅ Pega o tamanho real da imagem
ghost4_left.setTexture(texturaGhost4_left); // ✅ Define textura no sprite ghost4_left
ghost4_left.setOrigin(tamanhoGhost4_left.x / 2.0f, tamanhoGhost4_left.y / 2.0f); // ✅ Define origem no centro
ghost4_left.setScale(1.3f, 1.3f); // ✅ Aplica escala no sprite

//Ghost4_up

static sf::Texture texturaGhost4_up; // ✅ Criando textura para movimento para cima
if (!texturaGhost4_up.loadFromFile("resources/ghost4_up.png")) {
    std::cout << "Erro lendo imagem ghost4_up.png\n";
}
static sf::Vector2u tamanhoGhost4_up = texturaGhost4_up.getSize(); // ✅ Pega o tamanho real da imagem
ghost4_up.setTexture(texturaGhost4_up); // ✅ Define textura no sprite ghost4_up
ghost4_up.setOrigin(tamanhoGhost4_up.x / 2.0f, tamanhoGhost4_up.y / 2.0f); // ✅ Define origem no centro
ghost4_up.setScale(1.3f, 1.3f); // ✅ Aplica escala no sprite

//Ghost4_down

static sf::Texture texturaGhost4_down; // ✅ Criando textura para movimento para baixo
if (!texturaGhost4_down.loadFromFile("resources/ghost4_down.png")) {
    std::cout << "Erro lendo imagem ghost4_down.png\n";
}
static sf::Vector2u tamanhoGhost4_down = texturaGhost4_down.getSize(); // ✅ Pega o tamanho real da imagem
ghost4_down.setTexture(texturaGhost4_down); // ✅ Define textura no sprite ghost4_down
ghost4_down.setOrigin(tamanhoGhost4_down.x / 2.0f, tamanhoGhost4_down.y / 2.0f); // ✅ Define origem no centro
ghost4_down.setScale(1.3f, 1.3f); // ✅ Aplica escala no sprite
}