#include <SFML/Audio.hpp>  // <-- Adicionado para áudio
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string> 

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

int ultimaDirecaoGhost1 = -1; // Guarda a última direção válida de ghost para evitar voltar
int ultimaDirecaoGhost2 = -1; 

// Clocks para controlar tempo de movimento dos fantasmas
sf::Clock clockGhost1;
sf::Clock clockGhost2;
sf::Clock clockGhost3;
sf::Clock clockGhost4;
// cria a janela

bool jogoFinalizado = false; // situacao do jogo
bool musicaFinalPlay = false; // situacao da musica final

// PROTOTIPO DE FUNCOES
// Define as musicas do jogo
void DefineMusic(sf::Music &music, sf::Music &musicEndGame);
// Define os textos do jogo
void defineText (sf::Text &pontuacao, sf::Text &endGame, sf::Text &temporizador, sf::Font &font);
// Define os tiles para o fundo e parede
void defineTiles(sf::Sprite &spriteFundo, sf::Sprite &spriteParede);
// Define o sprite para as pilulas (pedras)
void definePedra(sf::Sprite &pedra);

// Define sprites do harry
void definePacman(sf::Sprite &pac, 
                  sf::Sprite &pac_up, 
                  sf::Sprite &pac_down, 
                  sf::Sprite &pac_left, 
                  sf::Sprite &pac_right);

// Define sprites dos fantasmas
void defineGhost(
    sf::Sprite &ghost1, sf::Sprite &ghost1_left, sf::Sprite &ghost1_up, sf::Sprite &ghost1_down, sf::Sprite &ghost1_right,
    sf::Sprite &ghost2, sf::Sprite &ghost2_left, sf::Sprite &ghost2_up, sf::Sprite &ghost2_down, sf::Sprite &ghost2_right,
    sf::Sprite &ghost3, sf::Sprite &ghost3_left, sf::Sprite &ghost3_up, sf::Sprite &ghost3_down, sf::Sprite &ghost3_right,
    sf::Sprite &ghost4, sf::Sprite &ghost4_left, sf::Sprite &ghost4_up, sf::Sprite &ghost4_down, sf::Sprite &ghost4_right);


// Função que verifica se uma coordenada (x, y) é válida para movimentação
// Retorna true se for dentro dos limites do mapa e não for parede
bool podeMover(int x, int y) {                                        // Verifica se x está entre 0 e 28 (colunas válidas do mapa)
   return x >= 0 && x < 29 && y >= 0 && y < 31 && mapa[y][x] != '1';  // Verifica se y está entre 0 e 30 (linhas válidas do mapa)
}                                                                     // Verifica se o caractere naquela posição não é '1' (parede)

// Verifica se o movimento é válido para fantasmas (impede reentrada na jaula

bool podeMoverGhost(int x, int y) {
    // Impede retorno à jaula (linhas 12 a 15, colunas 11 a 15)
    if ((y >= 12 && y <= 15) && (x >= 11 && x <= 15)) {
        return false;
    }

    // Impede entrada pela abertura da jaula (linha 12, coluna 13)
    if (y == 12 && x == 13) {
        return false;
    }

    return x >= 0 && x < 29 && y >= 0 && y < 31 && mapa[y][x] != '1';
}
// Libera fantasma da jaula seguindo um caminho até (13, 11) e depois sobe
void soltarGhost(bool &preso, sf::Clock &clock, float atraso,
                 double &x, double &y, sf::Sprite &sprite, sf::Sprite &sprite_up,
                 double destinoX = 13, double destinoY = 10) {
    if (!preso) return;

    if (clock.getElapsedTime().asSeconds() > atraso) {
        if ((int)y > destinoY && podeMover((int)x, (int)y - 1)) {
            y -= 1;
            sprite = sprite_up;
        } else if ((int)x < destinoX && podeMover((int)x + 1, (int)y)) {
            x += 1;
        } else if ((int)x > destinoX && podeMover((int)x - 1, (int)y)) {
            x -= 1;
        } else {
            preso = false;
        }
        clock.restart();
    }
}

int main() {
   int totalPedras = 0;
   int contadorPedra = 0;
   // Define as possiveis direcoes que o harry pode andar 
   enum Direcao { NENHUMA, ESQUERDA, DIREITA, CIMA, BAIXO };
   // Inicia a direçao seja nenhuma, ou seja, neutra e parada
   Direcao direcaoAtual = NENHUMA;

    sf::RenderWindow window(sf::VideoMode(980, 1085), "Pac-Wizard: A cacada em Hogwarts");

    // shape da parede
    sf::RectangleShape rectangle(sf::Vector2f(SIZE, SIZE));
    rectangle.setFillColor(sf::Color(0, 255, 255));
    rectangle.setOutlineThickness(-5);
    rectangle.setOutlineColor(sf::Color(50, 50, 50));
   
   // Criando os sprites do Harry em todas posições possiveis
   sf::Sprite pac, pac_up, pac_down, pac_left, pac_right;
   definePacman(pac, pac_up, pac_down, pac_left, pac_right);


    sf::Sprite pedra;
    definePedra(pedra);
   

    sf::Sprite ghost1;        //  Criando objeto sprite ghost1
    sf::Sprite ghost1_left;
    sf::Sprite ghost1_up;
    sf::Sprite ghost1_down;
    sf::Sprite ghost1_right;

    sf::Sprite ghost2;        //  Criando objeto sprite ghost2
    sf::Sprite ghost2_left;
    sf::Sprite ghost2_up;
    sf::Sprite ghost2_down;
    sf::Sprite ghost2_right;

    sf::Sprite ghost3;        //  Criando objeto sprite ghost3
    sf::Sprite ghost3_left;
    sf::Sprite ghost3_up;
    sf::Sprite ghost3_down;
    sf::Sprite ghost3_right;

    sf::Sprite ghost4;        //  Criando objeto sprite ghost4
    sf::Sprite ghost4_left;
    sf::Sprite ghost4_up;
    sf::Sprite ghost4_down;
    sf::Sprite ghost4_right;

    // Função para definir o sprite dos dementadores
    defineGhost(ghost1, ghost1_left, ghost1_up, ghost1_down, ghost1_right,
               ghost2, ghost2_left, ghost2_up, ghost2_down, ghost2_right,
               ghost3, ghost3_left, ghost3_up, ghost3_down, ghost3_right,
               ghost4, ghost4_left, ghost4_up, ghost4_down, ghost4_right);
 
   sf::Sprite spriteFundo; // Criando objeto sprite fundo
   sf::Sprite spriteParede; // Criando objeto sprite fund
   defineTiles(spriteFundo, spriteParede);


   // Música de fundo
   sf::Music music;
   sf::Music musicEndGame; // Musica de fim de jogo
   DefineMusic(music, musicEndGame);

   sf::Font font; // Carregando a fonte da pontuacao
   if (!font.loadFromFile("resources/fonts/upheavtt.ttf")) { 
   std::cout << "Erro carregando fonte\n";
   return 0;
   }

   // Conta a quantidade de pedras
   for(int i=0; i<31; i++)
      for(int j=0; j<28; j++){
         if(mapa[i][j]=='2'){
            totalPedras++;
      }
   }

   sf::Text pontuacao;
   sf::Text temporizador;
   sf::Text endGame;
   // Define os textos 
   defineText(pontuacao, temporizador, endGame, font);

   sf::Clock gameClock; // relogio do jogo
   sf::Clock clockAndar; // relogio do andar do harry

    while (window.isOpen()) {

        // verifica todos os eventos que foram acionados na janela desde a última iteração do loop
        sf::Event event;
        while (window.pollEvent(event)) {
            // evento "fechar" acionado: fecha a janela
            if (event.type == sf::Event::Closed)
                window.close();

            if (!jogoFinalizado && event.type == sf::Event::KeyPressed) { // se o jogo nao estiver finalizado e houver alguma tecla pressionada
               if (event.key.code == sf::Keyboard::Left) { // se for a tecla para esquerda
                  direcaoAtual = ESQUERDA; // direcao muda 
                  pac = pac_left; // sprite muda com base na direcao nova 
               }
               else if (event.key.code == sf::Keyboard::Right) {  // se for a tecla para direita
                  direcaoAtual = DIREITA;
                  pac = pac_right;
               }
               else if (event.key.code == sf::Keyboard::Up) {  // se for a tecla para cima
                  direcaoAtual = CIMA;
                  pac = pac_up;
               }
               else if (event.key.code == sf::Keyboard::Down) {  // se for a tecla para baixo
                  direcaoAtual = BAIXO;
                  pac = pac_down;
               }
            }
        }

      // a cada 0.2 segundos, atualiza a posicao automaticamente do pacman
      if (!jogoFinalizado && clockAndar.getElapsedTime() > sf::seconds(0.2)) {
      clockAndar.restart(); // reinicia o relogio para o próximo intervalo 
      // move o harry conforme a direção atual, se não houver parede, ou seja '1')
      switch (direcaoAtual) {
         case ESQUERDA:
               if (mapa[posy][posx-1] != '1') 
               posx--; // move para a esquerda no mapa
               else 
               direcaoAtual = NENHUMA; // bateu na parede, o movimento para
               break;
         case DIREITA:
               if (mapa[posy][posx+1] != '1') 
               posx++; // move para a direita no mapa
               else 
               direcaoAtual = NENHUMA;
               break;
         case CIMA:
               if (mapa[posy-1][posx] != '1') 
               posy--;  // move para cima no mapa
               else 
               direcaoAtual = NENHUMA;
               break;
         case BAIXO:
               if (mapa[posy+1][posx] != '1') 
               posy++; // move para baixo  no mapa
               else 
               direcaoAtual = NENHUMA;
               break;
         case NENHUMA:
               break; // não move
      }

    // verifica pedras e atualiza contador e fim de jogo
    if (mapa[posy][posx] == '2') { // se a posição atual do harry for '2', ou seja, a uma pedra
        mapa[posy][posx] = '0'; // atualiza e retira a pedra 
        contadorPedra++; // soma a quantidade de pedras coletada 
        std::cout << "Comidas: " << contadorPedra << std::endl; 
        if (contadorPedra == totalPedras)  // se o numero de pedras coletadas for igual a quantidade total do mapa
            jogoFinalizado = true; // a situacao do jogo muda e finaliza o jogo
    }
}

    //(teletransporte do pac de um lado para outo)
    if (posx < 0) posx = 27;     
    if (posx > 27) posx = 0; 

    // ------------------- Liberação dos fantasmas da jaula -------------------
// Chama a função soltarGhost para cada fantasma, que vai movê-los da jaula

soltarGhost(ghost1Preso, clockGhost1, 1.0f, ghost1X, ghost1Y, ghost1, ghost1_up, 13, 10);
soltarGhost(ghost2Preso, clockGhost2, 1.0f, ghost2X, ghost2Y, ghost2, ghost2_up, 13, 10);
soltarGhost(ghost3Preso, clockGhost3, 1.0f, ghost3X, ghost3Y, ghost3, ghost3_up, 13, 10);
soltarGhost(ghost4Preso, clockGhost4, 1.0f, ghost4X, ghost4Y, ghost4, ghost4_up, 13, 10);
  

// Movimento dos fantasmas 
// Só movemos cada fantasma se ele já tiver sido liberado da jaula (ghostXPreso == false).

// Movimento aleatório do ghost1

if (!ghost1Preso) {
    if (clockGhost1.getElapsedTime().asSeconds() > 0.1f) {

        // Vetores de deslocamento para cima, baixo, esquerda e direita
        int dx[] = {0, 0, -1, 1};
        int dy[] = {-1, 1, 0, 0};

        bool moveu = false; // Flag para saber se o fantasma conseguiu se mover

        for (int tentativas = 0; tentativas < 4 && !moveu; tentativas++) {
            int dir = rand() % 4; // Sorteia uma direção aleatória

            // Evita retornar para a direção oposta da anterior
            if ((dir == 0 && ultimaDirecaoGhost1 == 1) || // cima - baixo
                (dir == 1 && ultimaDirecaoGhost1 == 0) ||
                (dir == 2 && ultimaDirecaoGhost1 == 3) || // esquerda - direita
                (dir == 3 && ultimaDirecaoGhost1 == 2)) {
                continue;
            }

            int proxX = (int)ghost1X + dx[dir];
            int proxY = (int)ghost1Y + dy[dir];

            // Se a próxima posição for válida, realiza o movimento
            if (podeMoverGhost(proxX, proxY)) {
                ghost1X = proxX;
                ghost1Y = proxY;

                // Atualiza o sprite conforme a direção
                if (dir == 0) ghost1 = ghost1_up;
                else if (dir == 1) ghost1 = ghost1_down;
                else if (dir == 2) ghost1 = ghost1_left;
                else if (dir == 3) ghost1 = ghost1_right;

                ultimaDirecaoGhost1 = dir; // Salva a última direção usada
                moveu = true;
            }

        }

        clockGhost1.restart(); // Reinicia o relógio para próximo movimento
    }

// TELETRANSPORTE para ghost1 
if ((int)ghost1X <= 0) ghost1X = 26;
if ((int)ghost1X >= 27) ghost1X = 1;
}
   

// Movimento aleatório do ghost2 (igual ao ghost1)
if (!ghost2Preso) {
    if (clockGhost2.getElapsedTime().asSeconds() > 0.1f) {

        // Vetores de deslocamento para cima, baixo, esquerda e direita
        int dx[] = {0, 0, -1, 1};
        int dy[] = {-1, 1, 0, 0};

        bool moveu = false; // Flag para saber se o fantasma conseguiu se mover

        for (int tentativas = 0; tentativas < 4 && !moveu; tentativas++) {
            int dir = rand() % 4; // Sorteia uma direção aleatória

            // Evita retornar para a direção oposta da anterior
            if ((dir == 0 && ultimaDirecaoGhost2 == 1) || // cima - baixo
                (dir == 1 && ultimaDirecaoGhost2 == 0) ||
                (dir == 2 && ultimaDirecaoGhost2 == 3) || // esquerda - direita
                (dir == 3 && ultimaDirecaoGhost2 == 2)) {
                continue;
            }

            int proxX = (int)ghost2X + dx[dir];
            int proxY = (int)ghost2Y + dy[dir];

            // Se a próxima posição for válida, realiza o movimento
            if (podeMoverGhost(proxX, proxY)) {
                ghost2X = proxX;
                ghost2Y = proxY;

                // Atualiza o sprite conforme a direção
                if (dir == 0) ghost2 = ghost2_up;
                else if (dir == 1) ghost2 = ghost2_down;
                else if (dir == 2) ghost2 = ghost2_left;
                else if (dir == 3) ghost2 = ghost2_right;

                ultimaDirecaoGhost2 = dir; // Salva a última direção usada
                moveu = true;
            }

        }

        clockGhost2.restart(); // Reinicia o relógio para próximo movimento
    }
    // TELETRANSPORTE para ghost2
   if ((int)ghost2X <= 0) ghost2X = 26;
   if ((int)ghost2X >= 27) ghost2X = 1;

   }

// Movimento perseguidor do ghost3
// Prioriza se aproximar do Pacman no eixo X, depois no eixo Y
if (!ghost3Preso) {
    if (clockGhost3.getElapsedTime().asSeconds() > 0.3f) {
        int dx = 0, dy = 0;

        if ((int)ghost3X < posx && podeMoverGhost((int)ghost3X + 1, (int)ghost3Y)) dx = 1;
        else if ((int)ghost3X > posx && podeMoverGhost((int)ghost3X - 1, (int)ghost3Y)) dx = -1;
        else if ((int)ghost3Y < posy && podeMoverGhost((int)ghost3X, (int)ghost3Y + 1)) dy = 1;
        else if ((int)ghost3Y > posy && podeMoverGhost((int)ghost3X, (int)ghost3Y - 1)) dy = -1;

        // Atualiza sprite de acordo com direção do movimento
        if (dx == -1) ghost3 = ghost3_left;
        else if (dx == 1) ghost3 = ghost3_right;
        else if (dy == 1) ghost3 = ghost3_down;
        else if (dy == -1) ghost3 = ghost3_up;

        ghost3X += dx;
        ghost3Y += dy;

        clockGhost3.restart();
    }

    // TELETRANSPORTE para ghost3 (deve ficar aqui DENTRO do if !ghost3Preso)
    if ((int)ghost3X <= 0) ghost3X = 26;
    if ((int)ghost3X >= 27) ghost3X = 1;
}

// Movimento perseguidor do ghost4
// Prioriza se aproximar do Pacman no eixo Y, depois no eixo X
if (!ghost4Preso) {
    if (clockGhost4.getElapsedTime().asSeconds() > 0.3) {
        int dx = 0, dy = 0;

       if ((int)ghost4Y < posy && podeMoverGhost((int)ghost4X, (int)ghost4Y + 1)) dy = 1;
       else if ((int)ghost4Y > posy && podeMoverGhost((int)ghost4X, (int)ghost4Y - 1)) dy = -1;
       else if ((int)ghost4X < posx && podeMoverGhost((int)ghost4X + 1, (int)ghost4Y)) dx = 1;
       else if ((int)ghost4X > posx && podeMoverGhost((int)ghost4X - 1, (int)ghost4Y)) dx = -1;

        // Atualiza sprite conforme direção
        if (dx == -1) ghost4 = ghost4_left;
        else if (dx == 1) ghost4 = ghost4_right;
        else if (dy == 1) ghost4 = ghost4_down;
        else if (dy == -1) ghost4 = ghost4_up;

        ghost4X += dx;
        ghost4Y += dy;

        clockGhost4.restart();
    }
    // TELETRANSPORTE para ghost4
    if ((int)ghost4X <= 0) ghost4X = 26;
    if ((int)ghost4X >= 27) ghost4X = 1;
}
   // CONTEUDO DE TEXTO 

   // Temporizador
   // armazena o tempo ocorrido desde o inicio do clock em segundos
   int segundos = gameClock.getElapsedTime().asSeconds(); 
   // converte em minutos 
   int minutos = segundos / 60;
   // pega o que resta de segundos
   segundos = segundos % 60;
   temporizador.setString("Tempo: " + std::to_string(minutos) + ":" + (segundos < 10 ? "0" : "") + std::to_string(segundos));
   
   // Define o valor de pílulas comidas
   pontuacao.setString("Numero de Pedras Filosofais encontradas: " + std::to_string(contadorPedra));
   
   // Mensagem de fim de jogo
   endGame.setString("Patronus! Pedras coletadas! Hogwarts agradece, bravo(a) bruxo(a).");
   
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

     

      if(jogoFinalizado){
         window.draw(endGame);
         if(!musicaFinalPlay){
            music.stop();
            musicEndGame.setVolume(100);    // Volume de 0 a 100 (pode ajustar para menos se estiver muito alto)
            musicEndGame.play();         // Começa a tocar
            musicaFinalPlay = true;
         }
      }

      else{
         window.draw(pontuacao);
         window.draw(temporizador);
      }
        // termina e desenha o frame corrente
        window.display();
        
        }
    return 0;
}
void DefineMusic(sf::Music &music, sf::Music &musicEndGame){
   if (!music.openFromFile("resources/sounds/hp_sound.ogg")) {
   std::cout << "Erro ao carregar a música\n";
   }
    music.setLoop(true);  // Repetir a música
    music.setVolume(100);    // Volume de 0 a 100 (pode ajustar para menos se estiver muito alto)
    music.play();         // Começa a tocar
   
    // musica de fim de jogo
   if (!musicEndGame.openFromFile("resources/sounds/hp_endGame.ogg")) {
   std::cout << "Erro ao carregar a música\n";
   }

};
void defineText (sf::Text &pontuacao, sf::Text &temporizador, sf::Text &endGame, sf::Font &font){
   
   pontuacao.setFont(font);
   pontuacao.setCharacterSize(24); // Tamanho da fonte
   pontuacao.setFillColor(sf::Color(189,118,44)); // Cor da fonte
   pontuacao.setPosition(35, 10); // Posição na janela

   temporizador.setFont(font);
   temporizador.setCharacterSize(24); // Tamanho da fonte
   temporizador.setFillColor(sf::Color(189,118,44)); // Cor da fonte
   temporizador.setPosition(815, 10); // Posição na janela

   endGame.setFont(font);
   endGame.setCharacterSize(24); // tamanho da fonte
   endGame.setFillColor(sf::Color::Black); // cor da fonte
   endGame.setPosition(35, 5); // posição na janela
   endGame.setOutlineThickness(4); // tamanho da borda
   endGame.setOutlineColor(sf::Color(189,118,44)); // Cor da borda
}
void defineTiles(sf::Sprite &spriteFundo, sf::Sprite &spriteParede) {
   // Fundo
   static sf::Texture texturaFundo;  // Criando o objeto textura do fundo do jogo
   if (!texturaFundo.loadFromFile("resources/tiles/fundo.png")) {
      std::cout << "Erro lendo imagem fundo.png\n";
   }
   spriteFundo.setTexture(texturaFundo);

   // Parede
   static sf::Texture texturaParede;  // Criando o objeto textura da parede do jogo
   if (!texturaParede.loadFromFile("resources/tiles/parede.png")) {
      std::cout << "Erro lendo imagem parede.png\n";
   }
   spriteParede.setTexture(texturaParede);
}
void definePedra(sf::Sprite &pedra){
    // Sprite da Pedra (Pilula) -Textura
   static sf::Texture texturaPedra; // Criando o objeto textura da pedra
   if (!texturaPedra.loadFromFile("resources/pedra.png")) {
            std::cout << "Erro lendo imagem pedra.png\n";
    } //
   sf::Vector2u tamanhoTextura = texturaPedra.getSize(); // Pega o taamnho real da imagem (Textura da pedra)
     // Criando objeto spitepedra
    pedra.setTexture(texturaPedra); // atribuindo a textura
    pedra.setOrigin(tamanhoTextura.x / 2.0f, tamanhoTextura.y / 2.0f); // origem = centro // Divide o tamanho da celula para ir para o centro (f = float)
   pedra.setScale(0.8f, 0.8f);
}
void definePacman(sf::Sprite &pac, 
                  sf::Sprite &pac_up, 
                  sf::Sprite &pac_down, 
                  sf::Sprite &pac_left, 
                  sf::Sprite &pac_right){
    // sprite do PacMan
   static sf::Texture texturePac;
   if (!texturePac.loadFromFile("resources/hp.png")) {
      std::cout << "Erro lendo imagem hp.png\n";
   }
    pac.setTexture(texturePac); // atribuindo a textura
    sf::Vector2u tamanhoPac = texturePac.getSize(); // Pega o taamnho real da imagem (Textura do pacman)
    pac.setOrigin(tamanhoPac.x / 2.5f, tamanhoPac.y / 2.5f); // origem = centro // Divide o tamanho da celula para ir para o centro (f = float)
    // Tamanho do pacman
   pac.setScale(1.3f,1.3f);

   // Sprite do PacMan para cima (hp_up)
   static sf::Texture texturePac_up;
   if (!texturePac_up.loadFromFile("resources/hp_up.png")) {
      std::cout << "Erro lendo imagem hp_up.png\n";
   }
   pac_up.setTexture(texturePac_up);
   sf::Vector2u tamanhoPac_up = texturePac_up.getSize();
   pac_up.setOrigin(tamanhoPac_up.x / 2.5f, tamanhoPac_up.y / 2.5f);
   pac_up.setScale(1.3f, 1.3f);

   // Sprite do PacMan para baixo (hp_down)
   static sf::Texture texturePac_down;
   if (!texturePac_down.loadFromFile("resources/hp_down.png")) {
   std::cout << "Erro lendo imagem hp_down.png\n";
   }
   pac_down.setTexture(texturePac_down);
   sf::Vector2u tamanhoPac_down = texturePac_down.getSize();
   pac_down.setOrigin(tamanhoPac_down.x / 2.5f, tamanhoPac_down.y / 2.5f);
   pac_down.setScale(1.3f, 1.3f);

   // Sprite do PacMan para esquerda (hp_left)
   static sf::Texture texturePac_left;
   if (!texturePac_left.loadFromFile("resources/hp_left.png")) {
   std::cout << "Erro lendo imagem hp_left.png\n";
   }
   pac_left.setTexture(texturePac_left);
   sf::Vector2u tamanhoPac_left = texturePac_left.getSize();
   pac_left.setOrigin(tamanhoPac_left.x / 2.5f, tamanhoPac_left.y / 2.5f);
   pac_left.setScale(1.3f, 1.3f);

   // Sprite do PacMan para direita (hp_right)
   static sf::Texture texturePac_right;
   if (!texturePac_right.loadFromFile("resources/hp_right.png")) {
   std::cout << "Erro lendo imagem hp_right.png\n";
   }
   pac_right.setTexture(texturePac_right);
   sf::Vector2u tamanhoPac_right = texturePac_right.getSize();
   pac_right.setOrigin(tamanhoPac_right.x / 2.5f, tamanhoPac_right.y / 2.5f);
   pac_right.setScale(1.3f, 1.3f);
   
}
void defineGhost(
    sf::Sprite &ghost1, sf::Sprite &ghost1_left, sf::Sprite &ghost1_up, sf::Sprite &ghost1_down, sf::Sprite &ghost1_right,
    sf::Sprite &ghost2, sf::Sprite &ghost2_left, sf::Sprite &ghost2_up, sf::Sprite &ghost2_down, sf::Sprite &ghost2_right,
    sf::Sprite &ghost3, sf::Sprite &ghost3_left, sf::Sprite &ghost3_up, sf::Sprite &ghost3_down, sf::Sprite &ghost3_right,
    sf::Sprite &ghost4, sf::Sprite &ghost4_left, sf::Sprite &ghost4_up, sf::Sprite &ghost4_down, sf::Sprite &ghost4_right){


         // Sprites dos dementadores (ghost) - Textura

         //Ghost1

      static sf::Texture texturaGhost1; //  Criando o objeto textura do ghost1
      if (!texturaGhost1.loadFromFile("resources/ghost1.png")) { // Carregando imagem ghost1
         std::cout << "Erro lendo imagem ghost1.png\n"; //  Mensagem de erro
         // return 0;
      }
      static sf::Vector2u tamanhoGhost1 = texturaGhost1.getSize(); //  Pega o tamanho real
      //sf::Sprite ghost1; //  Criando objeto sprite ghost1
      ghost1.setTexture(texturaGhost1); // Define textura no sprite
      ghost1.setOrigin(tamanhoGhost1.x / 2.0f, tamanhoGhost1.y / 2.0f); //  Origem no centro
      ghost1.setScale(1.3f, 1.3f); //  Escala da imagem

      //Ghost1_right

      static sf::Texture texturaGhost1_right; // Criando textura para movimento para a direita
      if (!texturaGhost1_right.loadFromFile("resources/ghost1_right.png")) {
      std::cout << "Erro lendo imagem ghost1_right.png\n";
      }
      static sf::Vector2u tamanhoGhost1_right = texturaGhost1_right.getSize(); // Pega o tamanho real
      ghost1_right.setTexture(texturaGhost1_right); // Define textura
      ghost1_right.setOrigin(tamanhoGhost1_right.x / 2.0f, tamanhoGhost1_right.y / 2.0f); // Origem no centro
      ghost1_right.setScale(1.3f, 1.3f); // Escala

      //Ghost1_left

      static sf::Texture texturaGhost1_left; //  Criando textura para movimento à esquerda
      if (!texturaGhost1_left.loadFromFile("resources/ghost1_left.png")) {
         std::cout << "Erro lendo imagem ghost1_left.png\n";
      }
      static sf::Vector2u tamanhoGhost1_left = texturaGhost1_left.getSize(); //  Pega o tamanho real
      //sf::Sprite ghost1_left; //  Criando objeto sprite ghost1_left
      ghost1_left.setTexture(texturaGhost1_left); // Define textura
      ghost1_left.setOrigin(tamanhoGhost1_left.x / 2.0f, tamanhoGhost1_left.y / 2.0f); // Origem no centro
      ghost1_left.setScale(1.3f, 1.3f); // Escala

      //Ghost1_up

      static sf::Texture texturaGhost1_up; // Criando textura para movimento para cima
      if (!texturaGhost1_up.loadFromFile("resources/ghost1_up.png")) {
         std::cout << "Erro lendo imagem ghost1_up.png\n";
      }
      static sf::Vector2u tamanhoGhost1_up = texturaGhost1_up.getSize(); //  Pega o tamanho real
      //sf::Sprite ghost1_up; //  Criando objeto sprite ghost1_up
      ghost1_up.setTexture(texturaGhost1_up); //  Define textura
      ghost1_up.setOrigin(tamanhoGhost1_up.x / 2.0f, tamanhoGhost1_up.y / 2.0f); //  Origem no centro
      ghost1_up.setScale(1.3f, 1.3f); // Escala

      //Ghost1_down

      static sf::Texture texturaGhost1_down; //  Criando textura para movimento para baixo
      if (!texturaGhost1_down.loadFromFile("resources/ghost1_down.png")) {
         std::cout << "Erro lendo imagem ghost1_down.png\n";
      }
      static sf::Vector2u tamanhoGhost1_down = texturaGhost1_down.getSize(); //  Pega o tamanho real
      //sf::Sprite ghost1_down; //  Criando objeto sprite ghost1_down
      ghost1_down.setTexture(texturaGhost1_down); // Define textura
      ghost1_down.setOrigin(tamanhoGhost1_down.x / 2.0f, tamanhoGhost1_down.y / 2.0f); //  Origem no centro
      ghost1_down.setScale(1.3f, 1.3f); //  Escala

      //Ghost2

      static sf::Texture texturaGhost2; //  Criando o objeto textura do ghost2
      if (!texturaGhost2.loadFromFile("resources/ghost2.png")) {
         std::cout << "Erro lendo imagem ghost2.png\n";
      }
      static sf::Vector2u tamanhoGhost2 = texturaGhost2.getSize(); //  Pega o tamanho real
      //sf::Sprite ghost2;
      ghost2.setTexture(texturaGhost2); //  Define textura
      ghost2.setOrigin(tamanhoGhost2.x / 2.0f, tamanhoGhost2.y / 2.0f); //  Origem no centro
      ghost2.setScale(1.3f, 1.3f); //  Escala

       //Ghost2_right

      static sf::Texture texturaGhost2_right; // Criando textura para movimento para a direita
      if (!texturaGhost2_right.loadFromFile("resources/ghost2_right.png")) {
      std::cout << "Erro lendo imagem ghost2_right.png\n";
      }
      static sf::Vector2u tamanhoGhost2_right = texturaGhost2_right.getSize(); // Pega o tamanho real
      ghost2_right.setTexture(texturaGhost2_right); // Define textura
      ghost2_right.setOrigin(tamanhoGhost2_right.x / 2.0f, tamanhoGhost2_right.y / 2.0f); // Origem no centro
      ghost2_right.setScale(1.3f, 1.3f); // Escala

      //Ghost2_left

      static sf::Texture texturaGhost2_left;
      if (!texturaGhost2_left.loadFromFile("resources/ghost2_left.png")) {
         std::cout << "Erro lendo imagem ghost2_left.png\n";
      }
      static sf::Vector2u tamanhoGhost2_left = texturaGhost2_left.getSize(); //  Tamanho real
      ghost2_left.setTexture(texturaGhost2_left); //  Define textura
      ghost2_left.setOrigin(tamanhoGhost2_left.x / 2.0f, tamanhoGhost2_left.y / 2.0f); //  Origem no centro
      ghost2_left.setScale(1.3f, 1.3f); //  Escala

      //Ghost2_up

      static sf::Texture texturaGhost2_up; //  Criando textura para movimento para cima
      if (!texturaGhost2_up.loadFromFile("resources/ghost2_up.png")) {
         std::cout << "Erro lendo imagem ghost2_up.png\n";
      }
      static sf::Vector2u tamanhoGhost2_up = texturaGhost2_up.getSize(); //  Pega o tamanho real da imagem
      ghost2_up.setTexture(texturaGhost2_up); //  Define textura no sprite ghost2_up
      ghost2_up.setOrigin(tamanhoGhost2_up.x / 2.0f, tamanhoGhost2_up.y / 2.0f); //  Define origem no centro
      ghost2_up.setScale(1.3f, 1.3f); //  Aplica escala no sprite 

      //Ghost2_down

      static sf::Texture texturaGhost2_down; //  Criando textura para movimento para baixo
      if (!texturaGhost2_down.loadFromFile("resources/ghost2_down.png")) {
         std::cout << "Erro lendo imagem ghost2_down.png\n";
      }
      static sf::Vector2u tamanhoGhost2_down = texturaGhost2_down.getSize(); //  Pega o tamanho real da imagem
      ghost2_down.setTexture(texturaGhost2_down); //  Define textura no sprite ghost2_down
      ghost2_down.setOrigin(tamanhoGhost2_down.x / 2.0f, tamanhoGhost2_down.y / 2.0f); //  Define origem no centro
      ghost2_down.setScale(1.3f, 1.3f); //  Aplica escala no sprite

      //Ghost3

      static sf::Texture texturaGhost3; //  Criando textura do ghost3
      if (!texturaGhost3.loadFromFile("resources/ghost3.png")) {
         std::cout << "Erro lendo imagem ghost3.png\n";
      }
      static sf::Vector2u tamanhoGhost3 = texturaGhost3.getSize(); //  Pega o tamanho real da imagem
      ghost3.setTexture(texturaGhost3); //  Define textura no sprite ghost3
      ghost3.setOrigin(tamanhoGhost3.x / 2.0f, tamanhoGhost3.y / 2.0f); //  Define origem no centro
      ghost3.setScale(1.3f, 1.3f); //  Aplica escala no sprite
      //Ghost3_right

      static sf::Texture texturaGhost3_right; // Criando textura para movimento para a direita
      if (!texturaGhost3_right.loadFromFile("resources/ghost3_right.png")) {
      std::cout << "Erro lendo imagem ghost3_right.png\n"; // Mensagem de erro
      }
      static sf::Vector2u tamanhoGhost3_right = texturaGhost3_right.getSize(); // Pega o tamanho real da imagem
      ghost3_right.setTexture(texturaGhost3_right); // Define textura no sprite ghost3_right
      ghost3_right.setOrigin(tamanhoGhost3_right.x / 2.0f, tamanhoGhost3_right.y / 2.0f); // Define origem no centro
      ghost3_right.setScale(1.3f, 1.3f); // Aplica escala no sprite

      //Ghost3_left

      static sf::Texture texturaGhost3_left; //  Criando textura para movimento à esquerda
      if (!texturaGhost3_left.loadFromFile("resources/ghost3_left.png")) {
         std::cout << "Erro lendo imagem ghost3_left.png\n";
      }
      static sf::Vector2u tamanhoGhost3_left = texturaGhost3_left.getSize(); //  Pega o tamanho real da imagem
      ghost3_left.setTexture(texturaGhost3_left); //  Define textura no sprite ghost3_left
      ghost3_left.setOrigin(tamanhoGhost3_left.x / 2.0f, tamanhoGhost3_left.y / 2.0f); //  Define origem no centro
      ghost3_left.setScale(1.3f, 1.3f); //  Aplica escala no sprite

      //Ghost3_up

      static sf::Texture texturaGhost3_up; //  Criando textura para movimento para cima
      if (!texturaGhost3_up.loadFromFile("resources/ghost3_up.png")) {
         std::cout << "Erro lendo imagem ghost3_up.png\n";
      }
      static sf::Vector2u tamanhoGhost3_up = texturaGhost3_up.getSize(); //  Pega o tamanho real da imagem
      ghost3_up.setTexture(texturaGhost3_up); //  Define textura no sprite ghost3_up
      ghost3_up.setOrigin(tamanhoGhost3_up.x / 2.0f, tamanhoGhost3_up.y / 2.0f); //  Define origem no centro
      ghost3_up.setScale(1.3f, 1.3f); //  Aplica escala no sprite

      //Ghost3_down

      static sf::Texture texturaGhost3_down; //  Criando textura para movimento para baixo
      if (!texturaGhost3_down.loadFromFile("resources/ghost3_down.png")) {
         std::cout << "Erro lendo imagem ghost3_down.png\n";
      }
      static sf::Vector2u tamanhoGhost3_down = texturaGhost3_down.getSize(); //  Pega o tamanho real da imagem
      ghost3_down.setTexture(texturaGhost3_down); //  Define textura no sprite ghost3_down
      ghost3_down.setOrigin(tamanhoGhost3_down.x / 2.0f, tamanhoGhost3_down.y / 2.0f); //  Define origem no centro
      ghost3_down.setScale(1.3f, 1.3f); //  Aplica escala no sprite

      //Ghost4

      static sf::Texture texturaGhost4; //  Criando textura do ghost4
      if (!texturaGhost4.loadFromFile("resources/ghost4.png")) {
         std::cout << "Erro lendo imagem ghost4.png\n";
      }
      static sf::Vector2u tamanhoGhost4 = texturaGhost4.getSize(); //  Pega o tamanho real da imagem
      ghost4.setTexture(texturaGhost4); //  Define textura no sprite ghost4
      ghost4.setOrigin(tamanhoGhost4.x / 2.0f, tamanhoGhost4.y / 2.0f); //  Define origem no centro
      ghost4.setScale(1.3f, 1.3f); //  Aplica escala no sprite

      //Ghost4_right

      static sf::Texture texturaGhost4_right; // Criando textura para movimento para a direita
      if (!texturaGhost4_right.loadFromFile("resources/ghost4_right.png")) {
      std::cout << "Erro lendo imagem ghost4_right.png\n"; // Mensagem de erro
      }
      static sf::Vector2u tamanhoGhost4_right = texturaGhost4_right.getSize(); // Pega o tamanho real da imagem
      ghost4_right.setTexture(texturaGhost4_right); // Define textura no sprite ghost4_right
      ghost4_right.setOrigin(tamanhoGhost4_right.x / 2.0f, tamanhoGhost4_right.y / 2.0f); // Define origem no centro
      ghost4_right.setScale(1.3f, 1.3f); // Aplica escala no sprite

      //Ghost4_left

      static sf::Texture texturaGhost4_left; //  Criando textura para movimento à esquerda
      if (!texturaGhost4_left.loadFromFile("resources/ghost4_left.png")) {
         std::cout << "Erro lendo imagem ghost4_left.png\n";
      }
      static sf::Vector2u tamanhoGhost4_left = texturaGhost4_left.getSize(); //  Pega o tamanho real da imagem
      ghost4_left.setTexture(texturaGhost4_left); //  Define textura no sprite ghost4_left
      ghost4_left.setOrigin(tamanhoGhost4_left.x / 2.0f, tamanhoGhost4_left.y / 2.0f); //  Define origem no centro
      ghost4_left.setScale(1.3f, 1.3f); //  Aplica escala no sprite

      //Ghost4_up

      static sf::Texture texturaGhost4_up; //  Criando textura para movimento para cima
      if (!texturaGhost4_up.loadFromFile("resources/ghost4_up.png")) {
         std::cout << "Erro lendo imagem ghost4_up.png\n";
      }
      static sf::Vector2u tamanhoGhost4_up = texturaGhost4_up.getSize(); //  Pega o tamanho real da imagem
      ghost4_up.setTexture(texturaGhost4_up); //  Define textura no sprite ghost4_up
      ghost4_up.setOrigin(tamanhoGhost4_up.x / 2.0f, tamanhoGhost4_up.y / 2.0f); //  Define origem no centro
      ghost4_up.setScale(1.3f, 1.3f); //  Aplica escala no sprite

      //Ghost4_down

      static sf::Texture texturaGhost4_down; //  Criando textura para movimento para baixo
      if (!texturaGhost4_down.loadFromFile("resources/ghost4_down.png")) {
         std::cout << "Erro lendo imagem ghost4_down.png\n";
      }
      static sf::Vector2u tamanhoGhost4_down = texturaGhost4_down.getSize(); // Pega o tamanho real da imagem
      ghost4_down.setTexture(texturaGhost4_down); // Define textura no sprite ghost4_down
      ghost4_down.setOrigin(tamanhoGhost4_down.x / 2.0f, tamanhoGhost4_down.y / 2.0f); // Define origem no centro
      ghost4_down.setScale(1.3f, 1.3f); //  Aplica escala no sprite
      }