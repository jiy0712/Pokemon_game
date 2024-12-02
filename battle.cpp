#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>

void battle() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "pokemon_game");
    window.setFramerateLimit(60);

    sf::Music music;
    if (!music.openFromFile("bgm/shootingbattle.mp3")) {
        std::cerr << "음악 파일을 로드하지 못했습니다!" << std::endl;
        return;
    }
    music.setLoop(true);
    music.setVolume(30);
    music.play();

    //플레이어 이미지 (팽도리)
    sf::Texture player_texture, player_texture2;
    if (!player_texture.loadFromFile("img/paengdoriL.png") ||
        !player_texture2.loadFromFile("img/paengdoriR.png")) {
        std::cerr << "플레이어 이미지 로드 실패!" << std::endl;
        return;
    }

    sf::Sprite player_pokemon; //플레이어 포켓몬 스프라이트 생성
    player_pokemon.setTexture(player_texture); //텍스처 설정
    player_pokemon.setPosition(375, 50); //위치

    // 적 포켓몬 이미지
    std::vector<std::string> opponent_images = {
        "img/hankarias.png",
        "img/kongdulgi.png",
        "img/rijamong.png",
        "img/pikachu.png"
    };
    std::vector<sf::Texture> textures(opponent_images.size()); //적 텍스처 저장할 벡터
    for (size_t i = 0; i < opponent_images.size(); ++i) { //for문을 이용해 이미지 로드시키기
        if (!textures[i].loadFromFile(opponent_images[i])) {
            std::cerr << "적 이미지 로드 실패!" << std::endl;
            return;
        }
    }

    //게임 데이터
    std::vector<sf::CircleShape> skills; //공격기술 저장 벡터
    std::vector<sf::Sprite> opponent_pokemons; //적포켓몬 저장 벡터
    sf::Clock opponent_pokemon_timer, skillclock; //적 포켓몬 기술과 타이머 객체 생성
    int player_pokemon_hp = 5; //플레이어 초기 체력
    int score = 0; //초기 점수
    int max_score = 100; //스테이지 필요 점수
    int stage = 1; //초기 스테이지

    std::vector<sf::Sprite> hearts(5); //플레이어 포켓몬 체력 아이콘 저장 벡터
    sf::Texture heartTexture;
    if (!heartTexture.loadFromFile("img/hp.png")) {
        std::cerr << "체력 아이콘 이미지 로드 실패!" << std::endl;
        return;
    }

    for (int i = 0; i < 5; ++i) { //체력 아이콘 위치 설정
        sf::Sprite heart;
        heart.setTexture(heartTexture);
        heart.setPosition(10 + i * 30, 10);  //30px 간격으로 for문을 이용해 배치
        hearts[i] = heart;
    }

    //폰트
    sf::Font font;
    if (!font.loadFromFile("NotoSansKR.ttf")) {
        std::cerr << "폰트 로드 실패!" << std::endl;
        return;
    }

    //점수 텍스트
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(20);
    scoreText.setFillColor(sf::Color::Black);
    scoreText.setPosition(10, 50);

    //결과 텍스트 (gameover)
    sf::Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(50);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setString("GAME OVER");
    //중앙정렬
    sf::FloatRect gameOverBounds = gameOverText.getGlobalBounds();
    gameOverText.setPosition(
        (window.getSize().x - gameOverBounds.width) / 2,
        (window.getSize().y - gameOverBounds.height) / 2
    );

    //결과 텍스트 (win)
    sf::Text winText;
    winText.setFont(font);
    winText.setCharacterSize(50);
    winText.setFillColor(sf::Color::Green);
    winText.setString("YOU WIN!");
    //중앙정렬
    sf::FloatRect winBounds = winText.getGlobalBounds();
    winText.setPosition(
        (window.getSize().x - winBounds.width) / 2,
        (window.getSize().y - winBounds.height) / 2
    );

    //메인 루프
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (player_pokemon_hp > 0) { //플레이어 포켓몬의 체력이 0보다 크다면 (게임 진행 계속)
            float playerSpeed = 7.0f; //기본 플레이어 포켓몬 속도
            if (stage == 2) playerSpeed += 2;   //2단계 : 기본 속도 + 2
            if (stage == 3) playerSpeed *= 2;   //3단계 : 기본 속도 * 2

            //왼오 화살키를 통해 플레이어 이동
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && player_pokemon.getPosition().x > 0) //왼쪽
                player_pokemon.move(-playerSpeed, 0);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && player_pokemon.getPosition().x + player_pokemon.getGlobalBounds().width < 800) //오른쪽
                player_pokemon.move(playerSpeed, 0);

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && skillclock.getElapsedTime().asMilliseconds() > 250) { //스페이스 눌렀을 때
                sf::CircleShape skill(5.0f);
                skill.setFillColor(sf::Color::Blue);
                skill.setPosition(player_pokemon.getPosition().x + player_pokemon.getGlobalBounds().width / 2 - 2.5f, player_pokemon.getPosition().y + player_pokemon.getGlobalBounds().height);
                skills.push_back(skill);
                skillclock.restart(); //기술 발사 후 타이머 초기화 (타이머 : 기술 무자비한 사용 제한)

                //플레이어가 뛰는 듯한 느낌을 주기 위해
                if (player_pokemon.getTexture() == &player_texture) {
                    player_pokemon.setTexture(player_texture2); //공격하면 오른발이 앞으로
                }
                else {
                    player_pokemon.setTexture(player_texture); //아니면 왼발이 앞으로
                }//오른발 왼발 반복시켜 마치 걷는것을 보여준다
            }

            //적 포켓몬은 일정시간마다 등장한다
            if (opponent_pokemon_timer.getElapsedTime().asSeconds() > 1.0f - (stage - 1) * 0.2f) {
                const sf::Texture& random_texture = textures[rand() % textures.size()]; //랜덤으로 적 포켓몬 이미지 선택
                sf::Sprite opponent;
                opponent.setTexture(random_texture); //적 포켓몬 텍스처 설정
                opponent.setPosition(rand() % 750 + 25, 600); //랜덤 위치
                opponent_pokemons.push_back(opponent); //벡터 추가
                opponent_pokemon_timer.restart(); //적 포켓몬 등장 타이머 초기화
            }

            //스킬
            for (auto it = skills.begin(); it != skills.end();) {
                float skillSpeed = 8 + (stage - 1); //스킬 속도 증가
                it->move(0, skillSpeed); //스킬 이동
                if (it->getPosition().y > 600) { //화면 밖을 벗어나면 스킬 사라짐
                    it = skills.erase(it);
                }
                else {
                    ++it;
                }
            }

            //적 포켓몬의 이동과 화면 밖으로 나갔을 때
            for (auto it = opponent_pokemons.begin(); it != opponent_pokemons.end();) {
                if (stage == 2) {
                    it->move(0, -2 - 2); //2단계에서 속도 - 2 증가
                }
                else if (stage == 3) {
                    it->move(0, -2 * 2); //3단계에서 속도 2배
                }
                else {
                    it->move(0, -2); //기본속도 (1단계)
                }

                if (it->getPosition().y < 0) { //화면 밖으로 나갔다면 삭제
                    it = opponent_pokemons.erase(it);
                }
                else {
                    ++it;
                }
            }

            //기술과 적이 충돌했는지
            for (auto skill = skills.begin(); skill != skills.end();) {
                bool collided = false;
                for (auto opponent_pokemon = opponent_pokemons.begin(); opponent_pokemon != opponent_pokemons.end();) {
                    if (skill->getGlobalBounds().intersects(opponent_pokemon->getGlobalBounds())) { //충돌을 했다면
                        score += 10; //점수 10점
                        opponent_pokemon = opponent_pokemons.erase(opponent_pokemon); //충돌한 적 포켓몬은 없어짐
                        collided = true;
                        break;
                    }
                    else {
                        ++opponent_pokemon;
                    }
                }
                if (collided) {
                    skill = skills.erase(skill); //충돌된 스킬 없어짐
                }
                else {
                    ++skill;
                }
            }

            //적 포켓몬이 플레이어 포켓몬과 충돌하면
            for (auto it = opponent_pokemons.begin(); it != opponent_pokemons.end();) {
                if (player_pokemon.getGlobalBounds().intersects(it->getGlobalBounds())) { //충돌하면
                    player_pokemon_hp--; //체력 감소 (하트 1 없어짐)
                    it = opponent_pokemons.erase(it); //적 포켓몬 사라짐
                }
                else {
                    ++it;
                }
            }
            
            //점수가 최대에 도달했다면
            if (score >= max_score) {
                if (stage == 3) { //3단계라면
                    music.stop(); //음악 정리
                    window.clear(); //화면 지우기
                    window.draw(winText); //승리표시
                    window.display();
                    sf::sleep(sf::seconds(2)); //2초대기
                    window.close();
                    return;
                }
                //아직 1,2단계라면
                stage++; //다음단계로 이동
                score = 0; //점수 초기화
                max_score += 100; //다음 단계 도달 점수 증가 . 1단계 : 100점, 2단계 : 200점, 3단계 : 300점
                player_pokemon_hp = 5; //체력 초기화
            }
        }
        else { //체력이 0이라면
            music.stop();
            window.clear();
            window.draw(gameOverText); //실패표시
            window.display();
            sf::sleep(sf::seconds(2)); //2초대기
            window.close();
            return;
        }

        window.clear(sf::Color::White); //화면 흐니색 지우기

        for (int i = 0; i < player_pokemon_hp; ++i) window.draw(hearts[i]); //체력아이콘 그리기
        window.draw(player_pokemon); //플레이어 포켓몬 그리기
        for (auto& opponent_pokemon : opponent_pokemons) window.draw(opponent_pokemon); //적포켓몬 그리기
        for (auto& skill : skills) window.draw(skill); //기술 그리기

        //점수 텍스트 업데이트
        scoreText.setString("SCORE : " + std::to_string(score) + " / Level: " + std::to_string(stage));
        window.draw(scoreText); //점수 텍스트
        window.display();//점수를 화면 표시
    }
}
