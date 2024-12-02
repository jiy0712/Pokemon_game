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
        std::cerr << "음악 파일을 로드하지 못했습니다!" << std::endl;
        return;
    }
    music.setLoop(true);
    music.setVolume(30);
    music.play();

    //D-DAY 카운트다운
    sf::RenderWindow window(sf::VideoMode(800, 600), "pokemon_game");
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.loadFromFile("NotoSansKR.ttf")) {
        std::cerr << "폰트를 불러오는 데 실패했습니다." << std::endl;
        return;
    }

    sf::Text text("", font, 80);
    text.setFillColor(sf::Color::White);

    //텍스트 중앙정렬
    sf::FloatRect textBounds = text.getLocalBounds(); //텍스트 크기 정보
    text.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f); //텍스트 원점 중앙 설정
    text.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f); //화면 중앙 텍스트 배치

    int dDay = 30; //D-30
    sf::Clock clock; //카운트다운 시계 객체
    sf::Clock displayClock; //5초 후 화면 넘어가도록

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (clock.getElapsedTime().asSeconds() >= 0.1f && dDay > 0) { //0.1초마다 1씩 감소
            clock.restart();
            dDay--; //D-DAY 1씩 감소
        }

        //텍스트 표시
        sf::String displayText;
        if (dDay > 0)
            displayText = L"D-" + std::to_wstring(dDay); //문자열로 변환
        else
            displayText = L"D-DAY"; //0이 됐다면 D-DAY 표시

        text.setString(displayText); //텍스트 객체 문자열 설정


        //중앙정렬 (다시 정렬 D-DAY)
        textBounds = text.getLocalBounds();
        text.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
        text.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);

        window.clear(); //화면 지우기
        window.draw(text); //텍스트 그리기
        window.display(); //화면 업데이트

        if (displayClock.getElapsedTime().asSeconds() >= 5.0f) { //5초를 넘어가면 스토리로 넘어감
            break;
        }
    }

    //스토리
    window.create(sf::VideoMode(800, 600), "pokemon_game");
    window.setFramerateLimit(60);

    //대사
    std::vector<std::wstring> messages = {
        L"주인공 : 나와 팽도리는 강해졌다",               
        L"주인공 : 드디어 드래곤마스터와 대결할 날이 찾아왔다"          
    };

    //배경이미지
    sf::Texture background;
    if (!background.loadFromFile("img/boss.png")) {  
        std::cerr << "배경 이미지를 불러오는 데 실패했습니다.\n";
        return;
    }

    // 텍스트
    sf::Text dialogueText;
    dialogueText.setFont(font);
    dialogueText.setString("");
    dialogueText.setCharacterSize(22);
    dialogueText.setFillColor(sf::Color::Black);
    dialogueText.setPosition(40, 420);

    // 대화창
    sf::RectangleShape dialogueBox(sf::Vector2f(760, 140)); 
    dialogueBox.setFillColor(sf::Color(255, 255, 255, 220)); //반투명 흰색배경
    dialogueBox.setOutlineColor(sf::Color::Black);           //검정 테두리
    dialogueBox.setOutlineThickness(2);                      //테두리 두께
    dialogueBox.setPosition(20, 410);

    // 첫번째 화면
    sf::Sprite sprite(background); //배경 스프라이트 변환
    dialogueText.setString(messages[0]);

    // 화면 업데이트
    window.clear();
    window.draw(sprite);
    window.draw(dialogueBox);
    window.draw(dialogueText);
    window.display();

    // 엔터키를 친다면
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

    // 두번째 화면
    dialogueText.setString(messages[1]); //두번째 대사 보여주기

    // 화면 업데이트
    window.clear();
    window.draw(sprite); // 배경은 동일 (1,2 배경 같음)
    window.draw(dialogueBox);
    window.draw(dialogueText);
    window.display();

    // 엔터키를 친다면
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

    window.close(); // 창 닫기
}
