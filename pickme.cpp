#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp> //창 이벤트
#include <iostream>
#include <vector>
#include <string>
#include <locale> //지역화 지원
#include <SFML/Audio.hpp> //소리처리

void pickme() {
    //몬스터볼 구조체
    struct MonsterBall {
        sf::Sprite sprite; // 이미지
        sf::Text name; // 이름
    };

    sf::Music music;
    if (!music.openFromFile("bgm/pickmemusic.mp3")) {
        std::cerr << "음악 파일을 로드하지 못했습니다!" << std::endl;
        return;
    }
    music.setLoop(true);  
    music.setVolume(30);  
    music.play();

    sf::RenderWindow window(sf::VideoMode(800, 600), "pokemon_game");
    window.setFramerateLimit(60);

    // 폰트
    sf::Font font;
    if (!font.loadFromFile("NotoSansKR.ttf")) {
        return;
    }

    // 배경 이미지
    sf::Texture backgroundT;
    if (!backgroundT.loadFromFile("img/village.png")) {
        return;
    }
    sf::Sprite backgroundSprite(backgroundT); // 배경 스프라이트 설정

    // 몬스터볼 이미지
    sf::Texture monsterballT;
    if (!monsterballT.loadFromFile("img/monsterball.png")) {
        return;
    }

    // 몬스터볼, 몬스터볼 이름 벡터
    std::vector<MonsterBall> monsterball_img(3); // 3개 몬스터볼 이미지와 이름 저장할 공간
    std::vector<std::wstring> pokemonN = { L"팽도리", L"팽도리", L"팽도리" };

    // 몬스터볼 ( 이미지, 크기, 위치, 이름 )
    for (int i = 0; i < monsterball_img.size(); ++i) { //이미지 사이즈 만큼 반복(3)
        monsterball_img[i].sprite.setTexture(monsterballT); //몬스터볼 스프라이트 텍스처 설정
        monsterball_img[i].sprite.setScale(100.f / monsterballT.getSize().x, 100.f / monsterballT.getSize().y); //크기
        monsterball_img[i].sprite.setPosition(130 + i * 200, 300); //위치

        monsterball_img[i].name.setFont(font); //폰트
        monsterball_img[i].name.setString(pokemonN[i]); //이름
        monsterball_img[i].name.setCharacterSize(24); //폰트크기
        monsterball_img[i].name.setFillColor(sf::Color::Black); //폰트색상
        monsterball_img[i].name.setPosition(145 + i * 200, 400); //텍스트위치
    }

    int selectindex = 0; //선택한 포켓몬
    int confirmationindex = 0; //확인화면에서 선택한 포켓몬
    bool inConfirmation = false;

    while (window.isOpen()) { //창이 열려있는 동안
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (!inConfirmation) { //선택화면
                if (event.type == sf::Event::KeyPressed) { //키보드 설정
                    if (event.key.code == sf::Keyboard::Left) { //왼쪽방향키 누른다면
                        selectindex = (selectindex - 1 + monsterball_img.size()) % monsterball_img.size(); //선택한 인덱스 감소
                    }
                    else if (event.key.code == sf::Keyboard::Right) { //오른쪽방향키 누른다면
                        selectindex = (selectindex + 1) % monsterball_img.size(); //선택한 인덱스 증가
                    }
                    else if (event.key.code == sf::Keyboard::Enter) { //Enter 누른다면
                        inConfirmation = true; //확인 화면으로 이동
                    }
                }
            }
            else { //확인화면
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Up) { // Y(위쪽 화살표)
                        confirmationindex = 0;
                    }
                    else if (event.key.code == sf::Keyboard::Down) { // N(밑쪽 화살표)
                        confirmationindex = 1;
                    }
                    else if (event.key.code == sf::Keyboard::Enter) {
                        if (confirmationindex == 0) { // Y 선택
                            window.close(); //pickme 종료
                        }
                        else { // N 선택
                            inConfirmation = false; //선택화면으로 돌아감
                        }
                    }
                }
            }
        }

        window.clear(); //화면지움

        window.draw(backgroundSprite); // 배경이미지그리기

        if (!inConfirmation) { //선택화면 (렌더링)
            sf::Texture arrowTexture;
            if (!arrowTexture.loadFromFile("img/arrow.png")) { //화살표 텍스처 설정
                std::cerr << "Failed to load arrow image!" << std::endl;
                return;
            }
            sf::Sprite arrowSprite(arrowTexture);
            arrowSprite.setScale(30.f / arrowTexture.getSize().x, 30.f / arrowTexture.getSize().y); //크기
            arrowSprite.setPosition(monsterball_img[selectindex].sprite.getPosition().x + 30, monsterball_img[selectindex].sprite.getPosition().y - 40); //위치

            for (const auto& ball : monsterball_img) {
                window.draw(ball.sprite); //몬스터볼 이미지 그리기
                window.draw(ball.name); //몬스터 이름 그리기
            }

            window.draw(arrowSprite); //화살표 이미지 그리기
        }
        else { // 확인화면 (렌더링)
            sf::Font font;
            font.loadFromFile("NotoSansKR.ttf");

            sf::Text textP(L"이 포켓몬을 선택하시겠습니까?\n                    " + pokemonN[selectindex], font, 32); //확인질문
            textP.setFillColor(sf::Color::Black);
            sf::FloatRect titleBounds = textP.getLocalBounds();
            textP.setPosition(800 / 2 - titleBounds.width / 2, 600 / 2 - titleBounds.height / 2 - 100);

            // Y / N
            sf::Text Yoption(L"Y", font, 28);
            sf::Text Noption(L"N", font, 28);

            // 색상 변경
            if (confirmationindex == 0) { //Y가 선택되면 빨간색
                Yoption.setFillColor(sf::Color::Red);
                Noption.setFillColor(sf::Color::Black); //N은 검정색
            }
            else if (confirmationindex == 1) { //N이 선택되면 빨간색
                Yoption.setFillColor(sf::Color::Black); //Y는 검정색
                Noption.setFillColor(sf::Color::Red);
            }

            // 중앙 정렬
            sf::FloatRect yesB = Yoption.getLocalBounds();
            sf::FloatRect noB = Noption.getLocalBounds();
            Yoption.setPosition(800 / 2 - yesB.width / 2 - 20, 600 / 2 - yesB.height / 2 + 50);
            Noption.setPosition(800 / 2 - noB.width / 2 - 20, 600 / 2 - noB.height / 2 + 100);

            window.draw(textP); //제목 그리기
            window.draw(Yoption); //"Y" 그리기
            window.draw(Noption); //"N" 그리기
        }

        window.display(); //화면 재시작 (업데이트)
    }
}
