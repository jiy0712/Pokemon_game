    #include <SFML/Audio.hpp>
    #include <SFML/Graphics.hpp>
    #include <vector>
    #include <string>
    #include <thread>
    #include <chrono>
    #include <iostream>

    void plot1() {
        sf::Music music;
        if (!music.openFromFile("bgm/plot.mp3")) {
            std::cerr << "���� ������ �ε����� ���߽��ϴ�!" << std::endl;
            return;
        }
        music.setLoop(true);
        music.setVolume(30);
        music.play();

        sf::RenderWindow window(sf::VideoMode(800, 600), "pokemon_game");
        window.setFramerateLimit(60);

        //��ȭ �ؽ�Ʈ
        std::vector<std::wstring> messages = {
            L"���ΰ� : �ص����� ������",
            L"���ΰ� : �츮�� ������ �Ʒ��ߴ�",           
            L"���ΰ� : ���� �Ա⵵ �ϰ�",                 
            L"���ΰ� : ��ȸ�� ���� ���� �ߴ�",      
            L"���ΰ� : �̱����� ������ ������ �ִ�",      
            L"���ΰ� : ó�� �� ���� �츮 ��� �����߾���", 
            L"���ΰ� : �㿡 ħ�뿡�� ���� ���ڰ� ���� ������ �غ���", 
            L"���ΰ� : �׷��� ���� ����ϸ�",                   
            L"���ΰ� : ��ǥ�� �巡�︶���͸� ���� �� �����ϰ�\n���� ���ο� ����� ������", 
            L"���ΰ� : ��ȸ �� �޽��� ���ϸ� ü���� ȸ���ϰ�", 
            L"���ΰ� : �ᱹ ����� �� �ϸ� �巡�︶���Ϳ� �� ���������", 
            L"���ΰ� : ���� �巡�� �����͸� �̱涧���� �츮�� ��� �����Ѵ�" 
        };

        //����̹��� ȣ��
        std::vector<sf::Texture> backgrounds(messages.size());
        for (size_t i = 0; i < messages.size(); ++i) {
            std::string filename = "img/background" + std::to_string(i + 1) + ".png";
            if (!backgrounds[i].loadFromFile(filename)) {
                std::cerr << "Error: Unable to load " << filename << "\n";
                return;
            }
        }

        // ��Ʈ
        sf::Font font;
        if (!font.loadFromFile("NotoSansKR.ttf")) {
            std::cerr << "Error: Could not load font. Make sure the file is in the correct directory.\n";
            return;
        }

        // �ؽ�Ʈ
        sf::Text dialogueText;
        dialogueText.setFont(font);
        dialogueText.setString("");
        dialogueText.setCharacterSize(22);
        dialogueText.setFillColor(sf::Color::Black);
        dialogueText.setPosition(40, 420);

        // ��ȭâ ����
        sf::RectangleShape dialogueBox(sf::Vector2f(760, 140)); // ���� ũ�⸦ 10px ���� (����: 150)
        dialogueBox.setFillColor(sf::Color(255, 255, 255, 220)); // ������ ��� ���
        dialogueBox.setOutlineColor(sf::Color::Black);           // ������ �׵θ�
        dialogueBox.setOutlineThickness(2);                      // �׵θ� �β�
        dialogueBox.setPosition(20, 410); // ��ġ�� �������� ����

        // ��ȭ ǥ��
        for (size_t i = 0; i < messages.size(); ++i) {
            sf::Sprite background(backgrounds[i]);

            // �ؽ�Ʈ ������Ʈ
            dialogueText.setString(sf::String(messages[i]));

            // ȭ�� ������Ʈ
            window.clear();
            window.draw(background);
            window.draw(dialogueBox);
            window.draw(dialogueText);
            window.display();

            // Enter�� �����ٸ�
            bool enterPressed = false; //Enter ������ Ȯ���ϴ�
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
        }

        // ȭ�� ���� �� ��� ȭ�� ǥ��
        window.clear(sf::Color::White);

        // "���� ��Ÿ����!!!!" �ؽ�Ʈ ����
        sf::Text alertText;
        alertText.setFont(font);
        alertText.setString(L"���� ��Ÿ����!!!!");
        alertText.setCharacterSize(30);
        alertText.setFillColor(sf::Color::Red);
        alertText.setStyle(sf::Text::Bold);

        //�߾�����
        sf::FloatRect textBounds = alertText.getLocalBounds();
        alertText.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
        alertText.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);

        window.draw(alertText);
        window.display();

        //2�� ��� �� ����
        std::this_thread::sleep_for(std::chrono::seconds(2));

    }
