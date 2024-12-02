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
    backgroundSprite.setTexture(backgroundTexture); //��� ��������Ʈ �ؽ�ó ����

    sf::Texture paengdoriTexture;
    if (!paengdoriTexture.loadFromFile("img/paengdori_first_meeting.png")) {
        std::cerr << "Error: Could not load penguin image.\n";
        return;
    }
    sf::Sprite paengdoriSprite;
    paengdoriSprite.setTexture(paengdoriTexture); //���ϸ� ��������Ʈ �ؽ�ó ����
    paengdoriSprite.setPosition(300, 200);//��ġ
    paengdoriSprite.setScale(0.5f, 0.5f);//ũ��

    // ��ȭ
    sf::RectangleShape dialogueBox(sf::Vector2f(760, 150));
    dialogueBox.setFillColor(sf::Color(255, 255, 255, 220));
    dialogueBox.setOutlineColor(sf::Color::Black);
    dialogueBox.setOutlineThickness(2);
    dialogueBox.setPosition(20, 400);

    // ��Ʈ
    sf::Font font;
    if (!font.loadFromFile("NotoSansKR.ttf")) {
        std::cerr << "Error: Could not load font.\n";
        return;
    }

    // ��ȭ �ؽ�Ʈ
    sf::Text dialogueText;
    dialogueText.setFont(font);
    dialogueText.setCharacterSize(25);
    dialogueText.setFillColor(sf::Color::Black);
    dialogueText.setPosition(40, 420);

    // ������ �ؽ�Ʈ
    std::vector<sf::Text> optionTexts(3);
    for (size_t i = 0; i < optionTexts.size(); i++) {
        optionTexts[i].setFont(font);
        optionTexts[i].setCharacterSize(20);
        optionTexts[i].setFillColor(sf::Color::Black);
        optionTexts[i].setPosition(600, 420 + i * 30);
    }

    // ��ȭ ����
    std::vector<std::wstring> initialOptions = { L"�ȳ� �ص���", L"�Ϳ���", L"���� ª��...���׷�..." };
    std::vector<std::wstring> reactions = {
        L"����������������!!!",
        L"����������������~!~!~!",
        L"����..???????? ��������!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
    };
    std::vector<std::string> imagePaths = {
        "img/paengdori1_proud.png", // 1�� ����
        "img/paengdori2_love.png", // 2�� ����
        "img/paengdori3_wow.png",  // 3�� ����
        "img/paengdori3_sulky.png" // 3�� ���� �� ��� �� ����
    };

    std::vector<std::string> musicPaths = {
        "bgm/1.mp3", // 1�� �������
        "bgm/2.mp3", // 2�� �������
        "bgm/3.mp3"  // 3�� �������
    };

    std::wstring finalDialogue = L"���ΰ� : �츮 �� �������� !!";
    std::wstring apologyDialogue = L"�̾���..";
    int currentStep = 0;
    int selectedOptionIndex = 0;

    //�ܰ�, ���� �ɼ�
    bool inSelection = true;
    bool optionsVisible = true;
    bool apologyPhase = false;

    dialogueText.setString(L"��������!");
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
    bool isFirstMeetingPlaying = false; //�ݺ���� ���ϵ��� (1���� ����)

    while (window.isOpen()) { //â�� �����ִٸ�
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) { //Ű���� ������
                if (inSelection && optionsVisible) { //������
                    if (event.key.code == sf::Keyboard::Up) { //�� ����Ű
                        selectedOptionIndex = (selectedOptionIndex + initialOptions.size() - 1) % initialOptions.size(); //Y üũ
                    }
                    else if (event.key.code == sf::Keyboard::Down) { //�Ʒ� ����Ű
                        selectedOptionIndex = (selectedOptionIndex + 1) % initialOptions.size(); //Nüũ
                    }
                    else if (event.key.code == sf::Keyboard::Enter) { //Enter ������ ��
                        if (apologyPhase) { //����ܰ�
                            dialogueText.setString(L"��������,,,");
                            paengdoriTexture.loadFromFile(imagePaths[3]); //����̹���
                            paengdoriSprite.setTexture(paengdoriTexture); //�̹��� ��������Ʈ ����
                            optionsVisible = false; //������ ����
                        }
                        else { //����� �ƴ� ��
                            dialogueText.setString(reactions[selectedOptionIndex]); //�������� ���� ���� ����
                            paengdoriTexture.loadFromFile(imagePaths[selectedOptionIndex]); //�̹��� �ҷ�����
                            paengdoriSprite.setTexture(paengdoriTexture); //�̹��� ��������Ʈ ����

                            //�������� ���� ���� ���
                            if (selectedMusic.openFromFile(musicPaths[selectedOptionIndex])) {
                                selectedMusic.play();
                                selectedMusic.setLoop(false); //���� �ݺ� X
                            }

                            if (selectedOptionIndex == 2) { //����ϴ� ���� ������ ���
                                optionTexts[0].setString(apologyDialogue); //��� ����
                                optionTexts[1].setString(L""); //�������� �����
                                optionTexts[2].setString(L"");
                                apologyPhase = true;
                            }
                            else {
                                optionsVisible = false;
                            }
                        }
                    }
                }
                else if (!optionsVisible) { //������ ���� �Ⱥ��϶�
                    currentStep++; //�ܰ�����
                    if (currentStep == 1) { //1���̶��
                        dialogueText.setString(finalDialogue); //�� ��� ���
                    }
                    else if (currentStep == 2) { //2���̶��
                        window.close(); //���α׷� ����
                    }
                }
            }
        }

        // ���� �ٲ��
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

        // �ɼ� ���� ������Ʈ
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
        //���õ� �ɼ� ������ ����
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
