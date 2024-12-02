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
        std::cerr << "���� ������ �ε����� ���߽��ϴ�!" << std::endl;
        return;
    }
    music.setLoop(true);
    music.setVolume(30);
    music.play();

    //�÷��̾� �̹��� (�ص���)
    sf::Texture player_texture, player_texture2;
    if (!player_texture.loadFromFile("img/paengdoriL.png") ||
        !player_texture2.loadFromFile("img/paengdoriR.png")) {
        std::cerr << "�÷��̾� �̹��� �ε� ����!" << std::endl;
        return;
    }

    sf::Sprite player_pokemon; //�÷��̾� ���ϸ� ��������Ʈ ����
    player_pokemon.setTexture(player_texture); //�ؽ�ó ����
    player_pokemon.setPosition(375, 50); //��ġ

    // �� ���ϸ� �̹���
    std::vector<std::string> opponent_images = {
        "img/hankarias.png",
        "img/kongdulgi.png",
        "img/rijamong.png",
        "img/pikachu.png"
    };
    std::vector<sf::Texture> textures(opponent_images.size()); //�� �ؽ�ó ������ ����
    for (size_t i = 0; i < opponent_images.size(); ++i) { //for���� �̿��� �̹��� �ε��Ű��
        if (!textures[i].loadFromFile(opponent_images[i])) {
            std::cerr << "�� �̹��� �ε� ����!" << std::endl;
            return;
        }
    }

    //���� ������
    std::vector<sf::CircleShape> skills; //���ݱ�� ���� ����
    std::vector<sf::Sprite> opponent_pokemons; //�����ϸ� ���� ����
    sf::Clock opponent_pokemon_timer, skillclock; //�� ���ϸ� ����� Ÿ�̸� ��ü ����
    int player_pokemon_hp = 5; //�÷��̾� �ʱ� ü��
    int score = 0; //�ʱ� ����
    int max_score = 100; //�������� �ʿ� ����
    int stage = 1; //�ʱ� ��������

    std::vector<sf::Sprite> hearts(5); //�÷��̾� ���ϸ� ü�� ������ ���� ����
    sf::Texture heartTexture;
    if (!heartTexture.loadFromFile("img/hp.png")) {
        std::cerr << "ü�� ������ �̹��� �ε� ����!" << std::endl;
        return;
    }

    for (int i = 0; i < 5; ++i) { //ü�� ������ ��ġ ����
        sf::Sprite heart;
        heart.setTexture(heartTexture);
        heart.setPosition(10 + i * 30, 10);  //30px �������� for���� �̿��� ��ġ
        hearts[i] = heart;
    }

    //��Ʈ
    sf::Font font;
    if (!font.loadFromFile("NotoSansKR.ttf")) {
        std::cerr << "��Ʈ �ε� ����!" << std::endl;
        return;
    }

    //���� �ؽ�Ʈ
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(20);
    scoreText.setFillColor(sf::Color::Black);
    scoreText.setPosition(10, 50);

    //��� �ؽ�Ʈ (gameover)
    sf::Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(50);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setString("GAME OVER");
    //�߾�����
    sf::FloatRect gameOverBounds = gameOverText.getGlobalBounds();
    gameOverText.setPosition(
        (window.getSize().x - gameOverBounds.width) / 2,
        (window.getSize().y - gameOverBounds.height) / 2
    );

    //��� �ؽ�Ʈ (win)
    sf::Text winText;
    winText.setFont(font);
    winText.setCharacterSize(50);
    winText.setFillColor(sf::Color::Green);
    winText.setString("YOU WIN!");
    //�߾�����
    sf::FloatRect winBounds = winText.getGlobalBounds();
    winText.setPosition(
        (window.getSize().x - winBounds.width) / 2,
        (window.getSize().y - winBounds.height) / 2
    );

    //���� ����
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (player_pokemon_hp > 0) { //�÷��̾� ���ϸ��� ü���� 0���� ũ�ٸ� (���� ���� ���)
            float playerSpeed = 7.0f; //�⺻ �÷��̾� ���ϸ� �ӵ�
            if (stage == 2) playerSpeed += 2;   //2�ܰ� : �⺻ �ӵ� + 2
            if (stage == 3) playerSpeed *= 2;   //3�ܰ� : �⺻ �ӵ� * 2

            //�޿� ȭ��Ű�� ���� �÷��̾� �̵�
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && player_pokemon.getPosition().x > 0) //����
                player_pokemon.move(-playerSpeed, 0);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && player_pokemon.getPosition().x + player_pokemon.getGlobalBounds().width < 800) //������
                player_pokemon.move(playerSpeed, 0);

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && skillclock.getElapsedTime().asMilliseconds() > 250) { //�����̽� ������ ��
                sf::CircleShape skill(5.0f);
                skill.setFillColor(sf::Color::Blue);
                skill.setPosition(player_pokemon.getPosition().x + player_pokemon.getGlobalBounds().width / 2 - 2.5f, player_pokemon.getPosition().y + player_pokemon.getGlobalBounds().height);
                skills.push_back(skill);
                skillclock.restart(); //��� �߻� �� Ÿ�̸� �ʱ�ȭ (Ÿ�̸� : ��� ���ں��� ��� ����)

                //�÷��̾ �ٴ� ���� ������ �ֱ� ����
                if (player_pokemon.getTexture() == &player_texture) {
                    player_pokemon.setTexture(player_texture2); //�����ϸ� �������� ������
                }
                else {
                    player_pokemon.setTexture(player_texture); //�ƴϸ� �޹��� ������
                }//������ �޹� �ݺ����� ��ġ �ȴ°��� �����ش�
            }

            //�� ���ϸ��� �����ð����� �����Ѵ�
            if (opponent_pokemon_timer.getElapsedTime().asSeconds() > 1.0f - (stage - 1) * 0.2f) {
                const sf::Texture& random_texture = textures[rand() % textures.size()]; //�������� �� ���ϸ� �̹��� ����
                sf::Sprite opponent;
                opponent.setTexture(random_texture); //�� ���ϸ� �ؽ�ó ����
                opponent.setPosition(rand() % 750 + 25, 600); //���� ��ġ
                opponent_pokemons.push_back(opponent); //���� �߰�
                opponent_pokemon_timer.restart(); //�� ���ϸ� ���� Ÿ�̸� �ʱ�ȭ
            }

            //��ų
            for (auto it = skills.begin(); it != skills.end();) {
                float skillSpeed = 8 + (stage - 1); //��ų �ӵ� ����
                it->move(0, skillSpeed); //��ų �̵�
                if (it->getPosition().y > 600) { //ȭ�� ���� ����� ��ų �����
                    it = skills.erase(it);
                }
                else {
                    ++it;
                }
            }

            //�� ���ϸ��� �̵��� ȭ�� ������ ������ ��
            for (auto it = opponent_pokemons.begin(); it != opponent_pokemons.end();) {
                if (stage == 2) {
                    it->move(0, -2 - 2); //2�ܰ迡�� �ӵ� - 2 ����
                }
                else if (stage == 3) {
                    it->move(0, -2 * 2); //3�ܰ迡�� �ӵ� 2��
                }
                else {
                    it->move(0, -2); //�⺻�ӵ� (1�ܰ�)
                }

                if (it->getPosition().y < 0) { //ȭ�� ������ �����ٸ� ����
                    it = opponent_pokemons.erase(it);
                }
                else {
                    ++it;
                }
            }

            //����� ���� �浹�ߴ���
            for (auto skill = skills.begin(); skill != skills.end();) {
                bool collided = false;
                for (auto opponent_pokemon = opponent_pokemons.begin(); opponent_pokemon != opponent_pokemons.end();) {
                    if (skill->getGlobalBounds().intersects(opponent_pokemon->getGlobalBounds())) { //�浹�� �ߴٸ�
                        score += 10; //���� 10��
                        opponent_pokemon = opponent_pokemons.erase(opponent_pokemon); //�浹�� �� ���ϸ��� ������
                        collided = true;
                        break;
                    }
                    else {
                        ++opponent_pokemon;
                    }
                }
                if (collided) {
                    skill = skills.erase(skill); //�浹�� ��ų ������
                }
                else {
                    ++skill;
                }
            }

            //�� ���ϸ��� �÷��̾� ���ϸ�� �浹�ϸ�
            for (auto it = opponent_pokemons.begin(); it != opponent_pokemons.end();) {
                if (player_pokemon.getGlobalBounds().intersects(it->getGlobalBounds())) { //�浹�ϸ�
                    player_pokemon_hp--; //ü�� ���� (��Ʈ 1 ������)
                    it = opponent_pokemons.erase(it); //�� ���ϸ� �����
                }
                else {
                    ++it;
                }
            }
            
            //������ �ִ뿡 �����ߴٸ�
            if (score >= max_score) {
                if (stage == 3) { //3�ܰ���
                    music.stop(); //���� ����
                    window.clear(); //ȭ�� �����
                    window.draw(winText); //�¸�ǥ��
                    window.display();
                    sf::sleep(sf::seconds(2)); //2�ʴ��
                    window.close();
                    return;
                }
                //���� 1,2�ܰ���
                stage++; //�����ܰ�� �̵�
                score = 0; //���� �ʱ�ȭ
                max_score += 100; //���� �ܰ� ���� ���� ���� . 1�ܰ� : 100��, 2�ܰ� : 200��, 3�ܰ� : 300��
                player_pokemon_hp = 5; //ü�� �ʱ�ȭ
            }
        }
        else { //ü���� 0�̶��
            music.stop();
            window.clear();
            window.draw(gameOverText); //����ǥ��
            window.display();
            sf::sleep(sf::seconds(2)); //2�ʴ��
            window.close();
            return;
        }

        window.clear(sf::Color::White); //ȭ�� ��ϻ� �����

        for (int i = 0; i < player_pokemon_hp; ++i) window.draw(hearts[i]); //ü�¾����� �׸���
        window.draw(player_pokemon); //�÷��̾� ���ϸ� �׸���
        for (auto& opponent_pokemon : opponent_pokemons) window.draw(opponent_pokemon); //�����ϸ� �׸���
        for (auto& skill : skills) window.draw(skill); //��� �׸���

        //���� �ؽ�Ʈ ������Ʈ
        scoreText.setString("SCORE : " + std::to_string(score) + " / Level: " + std::to_string(stage));
        window.draw(scoreText); //���� �ؽ�Ʈ
        window.display();//������ ȭ�� ǥ��
    }
}
