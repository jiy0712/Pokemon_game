#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <SFML/Audio.hpp>

void lastbattle() {
    //HP (람다 함수. 크기/높이/위치/색상 등 설정)
    auto createHPBar = [](float maxWidth, float height, sf::Vector2f position, sf::Color color) -> sf::RectangleShape {
        sf::RectangleShape hpBar(sf::Vector2f(maxWidth, height));
        hpBar.setFillColor(color);
        hpBar.setPosition(position);
        return hpBar;
        };

    sf::Music music;
    if (!music.openFromFile("bgm/battle.mp3")) {
        std::cerr << "음악 파일을 로드하지 못했습니다!" << std::endl;
        return;
    }
    music.setLoop(true); 
    music.setVolume(30); 
    music.play();

    sf::RenderWindow window(sf::VideoMode(800, 600), "pokemon_game");
    std::srand(static_cast<unsigned>(std::time(0))); // 랜덤 시드 설정

    //배경 이미지
    sf::Texture bgTexture;
    if (!bgTexture.loadFromFile("img/background.png")) {
        std::cerr << "배경 이미지를 로드하지 못했습니다.\n";
        return;
    }
    sf::Sprite background(bgTexture);

    //플레이어 포켓몬
    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile("img/paengdori.png")) {
        std::cerr << "플레이어 포켓몬 이미지를 로드하지 못했습니다.\n";
        return;
    }
    sf::Sprite playerPokemon(playerTexture);
    playerPokemon.setPosition(100, 370);

    //상대 포켓몬
    sf::Texture opponentTexture;
    if (!opponentTexture.loadFromFile("img/hankaryas.png")) {
        std::cerr << "상대방 포켓몬 이미지를 로드하지 못했습니다.\n";
        return;
    }
    sf::Sprite opponentPokemon(opponentTexture);
    opponentPokemon.setPosition(560, 220);

    //체력 아이콘 5개
    std::vector<sf::Sprite> playerHearts(5);
    std::vector<sf::Sprite> opponentHearts(5);

    //체력 아이콘
    sf::Texture heartTexture;
    if (!heartTexture.loadFromFile("img/hp.png")) {
        std::cerr << "체력 아이콘 이미지 로드 실패!" << std::endl;
        return;
    }

    //플레이어와 상대 체력 아이콘 설정
    for (int i = 0; i < 5; ++i) {
        playerHearts[i].setTexture(heartTexture);
        playerHearts[i].setPosition(50 + i * 40, 300);  //플레이어의 체력 아이콘 위치

        opponentHearts[i].setTexture(heartTexture);
        opponentHearts[i].setPosition(550 + i * 40, 150);  //상대의 체력 아이콘 위치
    }

    //폰트
    sf::Font font;
    if (!font.loadFromFile("NotoSansKR.ttf")) {
        std::cerr << "폰트를 로드하지 못했습니다.\n";
        return;
    }

    //전투 메뉴 텍스트
    sf::Text menuText[3];
    std::string menuItems[] = { "Fight", "Pokemon", "Run" };
    for (int i = 0; i < 3; i++) {
        menuText[i].setFont(font); //폰트
        menuText[i].setString(menuItems[i]); //메뉴 항목
        menuText[i].setCharacterSize(30); //폰트크기
        menuText[i].setFillColor(sf::Color::White); //색상
        menuText[i].setPosition(50 + i * 200, 500); //위치
    }
    int selectedMenu = 0; //기본값으로 첫번째인 Fight로 지정
    menuText[selectedMenu].setFillColor(sf::Color::Yellow); //첫번째 메뉴 하이라이트

    //액션 알림
    sf::Text actionMessage("", font, 25);
    actionMessage.setFillColor(sf::Color::Red); //색상
    actionMessage.setPosition(200, 400); //위치
    bool isActionMessageVisible = false; //표시여부

    //게임 상태변수
    bool isGameOver = false; //게임오버여부
    bool isWin = false; //승리여부
    sf::Clock gameOverClock; //게임 오버 시간 관리용 시계

    //플레이어와 상대 HP 초기값
    int playerHP = 5;
    int opponentHP = 5;

    //메인 루프
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            //게임 오버가 아니라면
            if (!isGameOver) {
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Left) { //왼쪽으로 옮기면
                        menuText[selectedMenu].setFillColor(sf::Color::White); //현재 선택 해제
                        selectedMenu = (selectedMenu + 2) % 3; //왼쪽으로 이동해서 다른거 선택
                        menuText[selectedMenu].setFillColor(sf::Color::Yellow); //새로운 메뉴 항목 하이라이트
                    }
                    else if (event.key.code == sf::Keyboard::Right) { //오른쪽으로 옮기면
                        menuText[selectedMenu].setFillColor(sf::Color::White); //현재 선택 해제
                        selectedMenu = (selectedMenu + 1) % 3; //오른쪽으로 이동
                        menuText[selectedMenu].setFillColor(sf::Color::Yellow); //새로운 메뉴 항목 하이라이트
                    }

                    else if (event.key.code == sf::Keyboard::Enter) {
                        //메뉴 항목 선택 처리
                        if (selectedMenu == 0) { // Fight
                            //플레이어가 상대방을 공격
                            int playerDamage = std::rand() % 3 + 1; // 랜덤 대미지 1~3
                            opponentHP -= playerDamage; //상대 HP 감소
                            if (opponentHP < 0) opponentHP = 0;

                            //체력 아이콘 업데이트
                            for (int i = 0; i < 5; ++i) {
                                if (i >= opponentHP) {
                                    opponentHearts[i].setColor(sf::Color(100, 100, 100));  // 사라지게 처리
                                }
                            }

                            //상대방이 공격 (상대방이 아직 살아 있을 경우)
                            if (opponentHP > 0) {
                                int opponentAttackChance = std::rand() % 3;
                                if (opponentAttackChance == 0) { //3분의1 확률로 공격
                                    int opponentDamage = std::rand() % 3 + 1; //랜덤 대미지 1~3
                                    playerHP -= opponentDamage;
                                    if (playerHP < 0) playerHP = 0;

                                    //체력 아이콘 업데이트
                                    for (int i = 0; i < 5; ++i) {
                                        if (i >= playerHP) {
                                            playerHearts[i].setColor(sf::Color(100, 100, 100));  // 사라지게 처리
                                        }
                                    }
                                }
                            }

                            isActionMessageVisible = true;
                            actionMessage.setString(L"한카리아스가 대지미를 받았다");

                            // 게임 오버 체크
                            if (playerHP == 0) {
                                isGameOver = true;
                                isWin = false;
                                gameOverClock.restart();
                            }
                            else if (opponentHP == 0) {
                                isGameOver = true;
                                isWin = true;
                                gameOverClock.restart();
                            }
                        }
                        else if (selectedMenu == 1) { // Pokemon
                            isActionMessageVisible = true;
                            actionMessage.setString(L"현재 바꿀 포켓몬이 없습니다");
                        }
                        else if (selectedMenu == 2) { // Run
                            isActionMessageVisible = true;
                            actionMessage.setString(L"도망칠 수 없다!");
                        }
                    }
                }
            }
        }

        // 게임 오버 상태 처리
        if (isGameOver) {
            if (gameOverClock.getElapsedTime().asSeconds() > 2) {
                if (isWin) {
                    // 승리 화면 출력
                    sf::Text endMessage("Win!", font, 50);
                    endMessage.setFillColor(sf::Color::Black);

                    // 텍스트 중앙 정렬 계산
                    sf::FloatRect textBounds = endMessage.getLocalBounds();
                    endMessage.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
                    endMessage.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);

                    // 흰 화면 배경
                    window.clear(sf::Color::White);
                    window.draw(endMessage);
                    window.display();

                    sf::sleep(sf::seconds(2)); // 2초 대기 후 종료
                }
                else {
                    // 게임 오버 화면 출력
                    sf::Text endMessage("Game Over!", font, 50);
                    endMessage.setFillColor(sf::Color::Red);

                    // 텍스트 중앙 정렬 계산
                    sf::FloatRect textBounds = endMessage.getLocalBounds();
                    endMessage.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
                    endMessage.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);

                    // 검은 화면 배경
                    window.clear(sf::Color::Black);
                    window.draw(endMessage);
                    window.display();

                    sf::sleep(sf::seconds(2)); // 2초 대기 후 종료
                }

                window.close();
            }
        }

        window.clear();
        window.draw(background);

        //포켓몬과 체력 아이콘 그리기
        window.draw(playerPokemon);
        window.draw(opponentPokemon);

        for (auto& heart : playerHearts) {
            window.draw(heart); //플레이어 체력 아이콘
        }
        for (auto& heart : opponentHearts) {
            window.draw(heart); //상대 체력 아이콘
        }

        // 메뉴와 액션 메시지 출력
        for (int i = 0; i < 3; i++) {
            window.draw(menuText[i]); //메뉴 그리기
        }

        if (isActionMessageVisible) {
            window.draw(actionMessage); //액션알림
        }

        window.display();
    }
}
