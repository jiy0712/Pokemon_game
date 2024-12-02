#include <SFML/Graphics.hpp> //그래픽 SFML
#include <SFML/Window.hpp> //창 SFML
#include <SFML/Audio.hpp> //소리 SFML
#include <vector> //벡터
#include <string> //문자열
#include <iostream>

//함수를 main에서 호출할 수 있도록 세팅
void pickme();
void firstmeeting();
void plot1();
void battle();
void plot2();
void lastbattle();
void ending();

// 프로그램 시작
int main() {
    sf::Music music; //배경음악
    if (!music.openFromFile("bgm/startmusic.mp3")) {
        std::cerr << "음악 파일을 로드하지 못했습니다!" << std::endl;
        return -1;
    }
    music.setLoop(true); //무한반복
    music.setVolume(30); //볼륨 30%
    music.play();

    //창
    sf::RenderWindow window(sf::VideoMode(800, 600), "pokemon_game", sf::Style::Close);
    window.setFramerateLimit(60);

    //배경이미지
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("img/laboratory.png")) {
        std::cerr << "Error: Could not load background image. Make sure the file is in the correct directory.\n";
        return -1;
    }
    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture); //스프라이트 텍스처 설정

    // 대화
    sf::RectangleShape dialogueBox(sf::Vector2f(760, 150)); //창크기
    dialogueBox.setFillColor(sf::Color(255, 255, 255, 220)); //배경색
    dialogueBox.setOutlineColor(sf::Color::Black); //테두리색
    dialogueBox.setOutlineThickness(2); //테두리두께
    dialogueBox.setPosition(20, 400); //위치

    // 폰트
    sf::Font font;
    if (!font.loadFromFile("NotoSansKR.ttf")) {
        std::cerr << "Error: Could not load font. Make sure the file is in the correct directory.\n";
        return -1;
    }

    // 오박사 이미지
    sf::Texture portraitTexture;
    if (!portraitTexture.loadFromFile("img/doctorO.png")) {
        std::cerr << "Error: Could not load image. Make sure the file is in the correct directory.\n";
        return -1;
    }

    sf::Sprite portraitSprite; 
    portraitSprite.setTexture(portraitTexture); //스프라이트 텍스처 설정
    portraitSprite.setPosition(550, 140); //위치
    portraitSprite.setScale(0.5f, 0.5f); //0.5배 축소

    // 대화내용
    std::vector<std::wstring> dialogues = {
        L"흐음!! 잘 왔다!",
        L"포켓몬스터의 세계에 온 것을 환영한다!",
        L"내 이름은 오박사! 포켓몬 박사님이라 부르고 있단다",
        L"그건 그렇고 자네는 모험이 처음인겐가?",
        L"그럼 내가 이것저것 가르쳐주마!",
        L"이 세계에는 포켓몬스터",
        L"줄여서 '포켓몬'이라 불리는 신기한 생명체가 도시에 살고 있다!",
        L"우리 인간은 포켓몬과 사이좋게 살고 있지",
        L"함께 놀기도 하고 힘을 합쳐 일을 하기도 하고",
        L"그리고 포켓몬끼리 싸우게 하여 유대감을 돈독히 다지기도 하고...",
        L"나는 그런 포켓몬들을 자세히 알기 위해 연구하고 있는 것이란다!",
        L"자! 이제부터 너만의 이야기가 시작된다!",
        L"거기서 여러 포켓몬과 많은 사람들과 만나 무언가를 발견하게 되겠지!",
        L"그럼 포켓몬스터의 세계로!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!",
        L"포켓몬을 선택하세요 ~"
    };
    int dialogueIndex = -1;

    // 대화 텍스트 설정
    sf::Text dialogueText;
    dialogueText.setFont(font); //폰트적용
    dialogueText.setString(""); //초기 문자열
    dialogueText.setCharacterSize(22); //폰트크기
    dialogueText.setFillColor(sf::Color::Black); //폰트색상
    dialogueText.setPosition(40, 420); //텍스트 위치

    // MAIN
    while (window.isOpen()) { //창이 열려있는 동안
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) { //키보드 입력 이벤트
                if (event.key.code == sf::Keyboard::Enter) { //Enter를 눌렀을때
                    if (dialogueIndex < (int)dialogues.size() - 1) { //대화가 남았다면
                        dialogueIndex++;
                        dialogueText.setString(dialogues[dialogueIndex]); //다음대화
                        if (dialogueIndex == (int)dialogues.size() - 1) { //대화가 남지 않았다면 (마지막대화)
                            window.setVisible(false); // 메인 창 숨김

                            // 음악도 정지
                            if (music.getStatus() == sf::SoundSource::Playing) {
                                music.stop();
                            }

                            //다음 차례 함수 호출해 다른 파일 구현된 거 실행
                            pickme();       
                            firstmeeting();
                            plot1();
                            battle();
                            plot2();
                            lastbattle();
                            ending();
                        }
                    }
                }
            }
        }
        window.clear(); //화면 초기화
        window.draw(backgroundSprite); //배경
        window.draw(portraitSprite); //박사
        window.draw(dialogueBox); //대화창
        window.draw(dialogueText); //텍스트
        window.display(); //화면 재시작
    }

    return 0;
}
