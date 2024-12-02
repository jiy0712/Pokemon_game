#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>


void plot2() {
    sf::Music music;
    if (!music.openFromFile("bgm/plot.mp3")) {
        std::cerr << "���� ������ �ε����� ���߽��ϴ�!" << std::endl;
        return;
    }
    music.setLoop(true);
    music.setVolume(30);
    music.play();

    //D-DAY ī��Ʈ�ٿ�
    sf::RenderWindow window(sf::VideoMode(800, 600), "pokemon_game");
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.loadFromFile("NotoSansKR.ttf")) {
        std::cerr << "��Ʈ�� �ҷ����� �� �����߽��ϴ�." << std::endl;
        return;
    }

    sf::Text text("", font, 80);
    text.setFillColor(sf::Color::White);

    //�ؽ�Ʈ �߾�����
    sf::FloatRect textBounds = text.getLocalBounds(); //�ؽ�Ʈ ũ�� ����
    text.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f); //�ؽ�Ʈ ���� �߾� ����
    text.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f); //ȭ�� �߾� �ؽ�Ʈ ��ġ

    int dDay = 30; //D-30
    sf::Clock clock; //ī��Ʈ�ٿ� �ð� ��ü
    sf::Clock displayClock; //5�� �� ȭ�� �Ѿ����

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (clock.getElapsedTime().asSeconds() >= 0.1f && dDay > 0) { //0.1�ʸ��� 1�� ����
            clock.restart();
            dDay--; //D-DAY 1�� ����
        }

        //�ؽ�Ʈ ǥ��
        sf::String displayText;
        if (dDay > 0)
            displayText = L"D-" + std::to_wstring(dDay); //���ڿ��� ��ȯ
        else
            displayText = L"D-DAY"; //0�� �ƴٸ� D-DAY ǥ��

        text.setString(displayText); //�ؽ�Ʈ ��ü ���ڿ� ����


        //�߾����� (�ٽ� ���� D-DAY)
        textBounds = text.getLocalBounds();
        text.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
        text.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);

        window.clear(); //ȭ�� �����
        window.draw(text); //�ؽ�Ʈ �׸���
        window.display(); //ȭ�� ������Ʈ

        if (displayClock.getElapsedTime().asSeconds() >= 5.0f) { //5�ʸ� �Ѿ�� ���丮�� �Ѿ
            break;
        }
    }

    //���丮
    window.create(sf::VideoMode(800, 600), "pokemon_game");
    window.setFramerateLimit(60);

    //���
    std::vector<std::wstring> messages = {
        L"���ΰ� : ���� �ص����� ��������",               
        L"���ΰ� : ���� �巡�︶���Ϳ� ����� ���� ã�ƿԴ�"          
    };

    //����̹���
    sf::Texture background;
    if (!background.loadFromFile("img/boss.png")) {  
        std::cerr << "��� �̹����� �ҷ����� �� �����߽��ϴ�.\n";
        return;
    }

    // �ؽ�Ʈ
    sf::Text dialogueText;
    dialogueText.setFont(font);
    dialogueText.setString("");
    dialogueText.setCharacterSize(22);
    dialogueText.setFillColor(sf::Color::Black);
    dialogueText.setPosition(40, 420);

    // ��ȭâ
    sf::RectangleShape dialogueBox(sf::Vector2f(760, 140)); 
    dialogueBox.setFillColor(sf::Color(255, 255, 255, 220)); //������ ������
    dialogueBox.setOutlineColor(sf::Color::Black);           //���� �׵θ�
    dialogueBox.setOutlineThickness(2);                      //�׵θ� �β�
    dialogueBox.setPosition(20, 410);

    // ù��° ȭ��
    sf::Sprite sprite(background); //��� ��������Ʈ ��ȯ
    dialogueText.setString(messages[0]);

    // ȭ�� ������Ʈ
    window.clear();
    window.draw(sprite);
    window.draw(dialogueBox);
    window.draw(dialogueText);
    window.display();

    // ����Ű�� ģ�ٸ�
    bool enterPressed = false;
    while (window.isOpen() && !enterPressed) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            }
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                enterPressed = true;
            }
        }
    }

    // �ι�° ȭ��
    dialogueText.setString(messages[1]); //�ι�° ��� �����ֱ�

    // ȭ�� ������Ʈ
    window.clear();
    window.draw(sprite); // ����� ���� (1,2 ��� ����)
    window.draw(dialogueBox);
    window.draw(dialogueText);
    window.display();

    // ����Ű�� ģ�ٸ�
    enterPressed = false;
    while (window.isOpen() && !enterPressed) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            }
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                enterPressed = true;
            }
        }
    }

    window.close(); // â �ݱ�
}
