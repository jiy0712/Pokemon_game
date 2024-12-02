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
            std::cerr << "음악 파일을 로드하지 못했습니다!" << std::endl;
            return;
        }
        music.setLoop(true);
        music.setVolume(30);
        music.play();

        sf::RenderWindow window(sf::VideoMode(800, 600), "pokemon_game");
        window.setFramerateLimit(60);

        //대화 텍스트
        std::vector<std::wstring> messages = {
            L"주인공 : 팽도리를 만나고",
            L"주인공 : 우리는 열심히 훈련했다",           
            L"주인공 : 많이 먹기도 하고",                 
            L"주인공 : 대회에 나가 수상도 했다",      
            L"주인공 : 이긴적도 있지만 진적도 있다",      
            L"주인공 : 처음 진 날엔 우리 모두 좌절했었어", 
            L"주인공 : 밤에 침대에서 잠을 못자고 많이 생각도 해보고", 
            L"주인공 : 그래도 점점 노력하며",                   
            L"주인공 : 목표인 드래곤마스터를 위해 더 연습하고\n드디어 새로운 기술을 익혔다", 
            L"주인공 : 대회 전 휴식을 취하며 체력을 회복하고", 
            L"주인공 : 결국 우승을 또 하며 드래곤마스터에 더 가까워졌다", 
            L"주인공 : 현재 드래곤 마스터를 이길때까지 우리는 계속 모험한다" 
        };

        //배경이미지 호출
        std::vector<sf::Texture> backgrounds(messages.size());
        for (size_t i = 0; i < messages.size(); ++i) {
            std::string filename = "img/background" + std::to_string(i + 1) + ".png";
            if (!backgrounds[i].loadFromFile(filename)) {
                std::cerr << "Error: Unable to load " << filename << "\n";
                return;
            }
        }

        // 폰트
        sf::Font font;
        if (!font.loadFromFile("NotoSansKR.ttf")) {
            std::cerr << "Error: Could not load font. Make sure the file is in the correct directory.\n";
            return;
        }

        // 텍스트
        sf::Text dialogueText;
        dialogueText.setFont(font);
        dialogueText.setString("");
        dialogueText.setCharacterSize(22);
        dialogueText.setFillColor(sf::Color::Black);
        dialogueText.setPosition(40, 420);

        // 대화창 설정
        sf::RectangleShape dialogueBox(sf::Vector2f(760, 140)); // 세로 크기를 10px 줄임 (기존: 150)
        dialogueBox.setFillColor(sf::Color(255, 255, 255, 220)); // 반투명 흰색 배경
        dialogueBox.setOutlineColor(sf::Color::Black);           // 검은색 테두리
        dialogueBox.setOutlineThickness(2);                      // 테두리 두께
        dialogueBox.setPosition(20, 410); // 위치는 변경하지 않음

        // 대화 표시
        for (size_t i = 0; i < messages.size(); ++i) {
            sf::Sprite background(backgrounds[i]);

            // 텍스트 업데이트
            dialogueText.setString(sf::String(messages[i]));

            // 화면 업데이트
            window.clear();
            window.draw(background);
            window.draw(dialogueBox);
            window.draw(dialogueText);
            window.display();

            // Enter를 누른다면
            bool enterPressed = false; //Enter 누른지 확인하는
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

        // 화면 종료 전 흰색 화면 표시
        window.clear(sf::Color::White);

        // "적이 나타났어!!!!" 텍스트 설정
        sf::Text alertText;
        alertText.setFont(font);
        alertText.setString(L"적이 나타났어!!!!");
        alertText.setCharacterSize(30);
        alertText.setFillColor(sf::Color::Red);
        alertText.setStyle(sf::Text::Bold);

        //중앙정렬
        sf::FloatRect textBounds = alertText.getLocalBounds();
        alertText.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
        alertText.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);

        window.draw(alertText);
        window.display();

        //2초 대기 후 종료
        std::this_thread::sleep_for(std::chrono::seconds(2));

    }
