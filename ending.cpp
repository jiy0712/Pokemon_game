#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>

void ending() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "pokemon_game");
    window.setFramerateLimit(60);

    //����̹���
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("img/ending.png")) {
        std::cerr << "Failed to load ending.png" << std::endl;
        return;
    }
    sf::Sprite background(backgroundTexture);
    background.setColor(sf::Color(255, 255, 255, 0)); //��� ���� 0���� ���� (������ ���̰�)

    //��Ʈ
    sf::Font font;
    if (!font.loadFromFile("NotoSansKR.ttf")) { // ����� �ѱ� ��Ʈ
        std::cerr << "Failed to load font" << std::endl;
        return;
    }

    //��ȭâ
    sf::RectangleShape dialogueBox(sf::Vector2f(760, 150)); //ũ��
    dialogueBox.setFillColor(sf::Color(255, 255, 255, 220)); //����
    dialogueBox.setOutlineColor(sf::Color::Black); //�ܰ��� ��
    dialogueBox.setOutlineThickness(2); //�ܰ��� �β�
    dialogueBox.setPosition(20, 400); //��ġ

    // �ؽ�Ʈ
    sf::Text dialogueText;
    dialogueText.setFont(font);
    dialogueText.setString(L"���ǿ���\n�� ������ ���丮�� ���� �������� ��忣���� �����ϴ�"); 
    dialogueText.setCharacterSize(24);
    dialogueText.setFillColor(sf::Color::Black);
    dialogueText.setPosition(40, 420); //��ġ

    float alphaBackground = 0; // ��� ����

    // ���η���
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            //����Ű�� ������ ���α׷� ����
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                window.close();
            }
        }

        // ��� ���� ����
        if (alphaBackground < 255) {
            alphaBackground += 1; // õõ�� ����
            background.setColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(alphaBackground)));
        }

        // ȭ�鿡 �׸���
        window.clear(); //�ʱ�ȭ
        window.draw(background); //��� �׸���
        window.draw(dialogueBox); //��ǳ�� �׸���
        window.draw(dialogueText); //�ؽ�Ʈ �׸���
        window.display(); //������Ʈ
    }
}
