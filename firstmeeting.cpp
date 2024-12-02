#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <string>

void firstmeeting() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "pokemon_game", sf::Style::Close);
    window.setFramerateLimit(60);

    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("img/village.png")) {
        std::cerr << "Error: Could not load background image.\n";
        return;
    }
    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture); //배경 스프라이트 텍스처 설정

    sf::Texture paengdoriTexture;
    if (!paengdoriTexture.loadFromFile("img/paengdori_first_meeting.png")) {
        std::cerr << "Error: Could not load penguin image.\n";
        return;
    }
    sf::Sprite paengdoriSprite;
    paengdoriSprite.setTexture(paengdoriTexture); //포켓몬 스프라이트 텍스처 설정
    paengdoriSprite.setPosition(300, 200);//위치
    paengdoriSprite.setScale(0.5f, 0.5f);//크기

    // 대화
    sf::RectangleShape dialogueBox(sf::Vector2f(760, 150));
    dialogueBox.setFillColor(sf::Color(255, 255, 255, 220));
    dialogueBox.setOutlineColor(sf::Color::Black);
    dialogueBox.setOutlineThickness(2);
    dialogueBox.setPosition(20, 400);

    // 폰트
    sf::Font font;
    if (!font.loadFromFile("NotoSansKR.ttf")) {
        std::cerr << "Error: Could not load font.\n";
        return;
    }

    // 대화 텍스트
    sf::Text dialogueText;
    dialogueText.setFont(font);
    dialogueText.setCharacterSize(25);
    dialogueText.setFillColor(sf::Color::Black);
    dialogueText.setPosition(40, 420);

    // 선택지 텍스트
    std::vector<sf::Text> optionTexts(3);
    for (size_t i = 0; i < optionTexts.size(); i++) {
        optionTexts[i].setFont(font);
        optionTexts[i].setCharacterSize(20);
        optionTexts[i].setFillColor(sf::Color::Black);
        optionTexts[i].setPosition(600, 420 + i * 30);
    }

    // 대화 문구
    std::vector<std::wstring> initialOptions = { L"안녕 팽도리", L"귀엽다", L"왤케 짧아...동그래..." };
    std::vector<std::wstring> reactions = {
        L"도리도리도리도리!!!",
        L"도리도리도리도리~!~!~!",
        L"도리..???????? 도리도리!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
    };
    std::vector<std::string> imagePaths = {
        "img/paengdori1_proud.png", // 1번 선택
        "img/paengdori2_love.png", // 2번 선택
        "img/paengdori3_wow.png",  // 3번 선택
        "img/paengdori3_sulky.png" // 3번 선택 후 사과 후 선택
    };

    std::vector<std::string> musicPaths = {
        "bgm/1.mp3", // 1번 배경음악
        "bgm/2.mp3", // 2번 배경음악
        "bgm/3.mp3"  // 3번 배경음악
    };

    std::wstring finalDialogue = L"주인공 : 우리 잘 지내보자 !!";
    std::wstring apologyDialogue = L"미안해..";
    int currentStep = 0;
    int selectedOptionIndex = 0;

    //단계, 선택 옵션
    bool inSelection = true;
    bool optionsVisible = true;
    bool apologyPhase = false;

    dialogueText.setString(L"도리도리!");
    for (size_t i = 0; i < initialOptions.size(); i++) {
        optionTexts[i].setString(initialOptions[i]);
    }

    sf::Music music;
    sf::Music firstMeetingMusic;
    sf::Music selectedMusic;
    if (!music.openFromFile("bgm/get.mp3")) {
        std::cerr << "Error: Could not load bgm/get.mp3.\n";
        return;
    }
    if (!firstMeetingMusic.openFromFile("bgm/firstmeeting.mp3")) {
        std::cerr << "Error: Could not load bgm/firstmeeting.mp3.\n";
        return;
    }

    music.play();
    music.setLoop(false);

    sf::Clock musicClock;
    bool isFirstMeetingPlaying = false; //반복재생 안하도록 (1번만 실행)

    while (window.isOpen()) { //창이 열려있다면
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) { //키보드 누르면
                if (inSelection && optionsVisible) { //선택지
                    if (event.key.code == sf::Keyboard::Up) { //위 방향키
                        selectedOptionIndex = (selectedOptionIndex + initialOptions.size() - 1) % initialOptions.size(); //Y 체크
                    }
                    else if (event.key.code == sf::Keyboard::Down) { //아래 방향키
                        selectedOptionIndex = (selectedOptionIndex + 1) % initialOptions.size(); //N체크
                    }
                    else if (event.key.code == sf::Keyboard::Enter) { //Enter 눌렀을 때
                        if (apologyPhase) { //사과단계
                            dialogueText.setString(L"도리도리,,,");
                            paengdoriTexture.loadFromFile(imagePaths[3]); //사과이미지
                            paengdoriSprite.setTexture(paengdoriTexture); //이미지 스프라이트 적용
                            optionsVisible = false; //선택지 숨김
                        }
                        else { //사과가 아닐 때
                            dialogueText.setString(reactions[selectedOptionIndex]); //선택지에 따른 반응 설정
                            paengdoriTexture.loadFromFile(imagePaths[selectedOptionIndex]); //이미지 불러오기
                            paengdoriSprite.setTexture(paengdoriTexture); //이미지 스프라이트 적용

                            //선택지에 따라 음악 재생
                            if (selectedMusic.openFromFile(musicPaths[selectedOptionIndex])) {
                                selectedMusic.play();
                                selectedMusic.setLoop(false); //음악 반복 X
                            }

                            if (selectedOptionIndex == 2) { //사과하는 것을 선택한 경우
                                optionTexts[0].setString(apologyDialogue); //대사 변경
                                optionTexts[1].setString(L""); //선택지들 숨기기
                                optionTexts[2].setString(L"");
                                apologyPhase = true;
                            }
                            else {
                                optionsVisible = false;
                            }
                        }
                    }
                }
                else if (!optionsVisible) { //선택지 이제 안보일때
                    currentStep++; //단계증가
                    if (currentStep == 1) { //1번이라면
                        dialogueText.setString(finalDialogue); //이 대사 사용
                    }
                    else if (currentStep == 2) { //2번이라면
                        window.close(); //프로그램 종료
                    }
                }
            }
        }

        // 음악 바뀌는
        if (!isFirstMeetingPlaying && music.getStatus() == sf::SoundSource::Stopped) {
            if (musicClock.getElapsedTime().asSeconds() >= 0.5f) {
                firstMeetingMusic.play();
                firstMeetingMusic.setLoop(true);
                isFirstMeetingPlaying = true;
            }
        }
        else if (!isFirstMeetingPlaying) {
            musicClock.restart();
        }

        // 옵션 변경 업데이트
        if (optionsVisible) {
            for (size_t i = 0; i < optionTexts.size(); i++) {
                optionTexts[i].setFillColor(i == selectedOptionIndex ? sf::Color::Red : sf::Color::Black);
            }
        }

        window.clear();
        window.draw(backgroundSprite);
        window.draw(dialogueBox);
        window.draw(paengdoriSprite);
        window.draw(dialogueText);
        //선택된 옵션 빨갛게 설정
        if (optionsVisible) {
            for (const auto& option : optionTexts) {
                if (!option.getString().isEmpty())
                    window.draw(option);
            }
        }
        window.display();
    }

    return;
}
