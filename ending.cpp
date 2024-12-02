#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>

void ending() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "pokemon_game");
    window.setFramerateLimit(60);

    //배경이미지
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("img/ending.png")) {
        std::cerr << "Failed to load ending.png" << std::endl;
        return;
    }
    sf::Sprite background(backgroundTexture);
    background.setColor(sf::Color(255, 255, 255, 0)); //배경 투명도 0으로 시작 (서서히 보이게)

    //폰트
    sf::Font font;
    if (!font.loadFromFile("NotoSansKR.ttf")) { // 사용할 한글 폰트
        std::cerr << "Failed to load font" << std::endl;
        return;
    }

    //대화창
    sf::RectangleShape dialogueBox(sf::Vector2f(760, 150)); //크기
    dialogueBox.setFillColor(sf::Color(255, 255, 255, 220)); //배경색
    dialogueBox.setOutlineColor(sf::Color::Black); //외곽선 색
    dialogueBox.setOutlineThickness(2); //외곽선 두께
    dialogueBox.setPosition(20, 400); //위치

    // 텍스트
    sf::Text dialogueText;
    dialogueText.setFont(font);
    dialogueText.setString(L"해피엔딩\n이 게임은 스토리형 성장 게임으로 배드엔딩이 없습니다"); 
    dialogueText.setCharacterSize(24);
    dialogueText.setFillColor(sf::Color::Black);
    dialogueText.setPosition(40, 420); //위치

    float alphaBackground = 0; // 배경 투명도

    // 메인루프
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            //엔터키가 눌리면 프로그램 종료
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                window.close();
            }
        }

        // 배경 투명도 증가
        if (alphaBackground < 255) {
            alphaBackground += 1; // 천천히 증가
            background.setColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(alphaBackground)));
        }

        // 화면에 그리기
        window.clear(); //초기화
        window.draw(background); //배경 그리기
        window.draw(dialogueBox); //말풍선 그리기
        window.draw(dialogueText); //텍스트 그리기
        window.display(); //업데이트
    }
}
