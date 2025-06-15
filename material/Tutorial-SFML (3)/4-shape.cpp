#include <SFML/Graphics.hpp>
#include <cstdlib>
int main() {
    // cria a janela
    sf::RenderWindow window(sf::VideoMode(800, 600), "Minha janela");

    // cria um círculo de raio 50
    sf::CircleShape circ2(50);
    circ2.setPosition(350, 250);
   
    sf::Vector2f tamanhoRet(100, 50);
    sf::RectangleShape ret1(tamanhoRet);
    sf::RectangleShape ret2(tamanhoRet);
    sf::RectangleShape ret3(tamanhoRet);
    sf::RectangleShape ret4(tamanhoRet);
    ret1.setFillColor(sf::Color::Red);
    ret2.setFillColor(sf::Color::Green);
    ret3.setFillColor(sf::Color::Blue);
    ret4.setFillColor(sf::Color::Yellow);
    ret1.setPosition(0, 0); // canto superior esquerdo
    ret2.setPosition(700,0); // canto superior direito
    ret3.setPosition(0, 550); // canto inferior esquerdo
    ret4.setPosition(700, 550); // canto inferior direito

    while (window.isOpen()) {

        // verifica todos os eventos que foram acionados na janela desde a última iteração do loop
        sf::Event event;
        while (window.pollEvent(event)) {
            // evento "fechar" acionado: fecha a janela
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // limpa a janela com a cor preta
        window.clear(sf::Color::Black);
         circ2.setFillColor(sf::Color(rand()%256, rand()%256, rand()%256)); // cor 
        // desenhar tudo aqui...

        // desenha o círculo na janela
        window.draw(circ2);
        window.draw(ret1);
        window.draw(ret2);
        window.draw(ret3);
        window.draw(ret4);
        // termina e desenha o frame corrente
        window.display();
    }

    return 0;
}
